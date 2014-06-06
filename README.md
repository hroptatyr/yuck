yuck
====

[![Build Status](https://secure.travis-ci.org/hroptatyr/yuck.png?branch=master)](http://travis-ci.org/hroptatyr/yuck)
[![Build Status](https://drone.io/github.com/hroptatyr/yuck/status.png)](https://drone.io/github.com/hroptatyr/yuck/latest)

+ project homepage: http://www.fresse.org/yuck/
+ github page: https://github.com/hroptatyr/yuck
+ downloads: https://github.com/hroptatyr/yuck/releases
+ issues: https://github.com/hroptatyr/yuck/issues

### Your Umbrella Command Kit

yuck is a bog-standard command line option parser for C that works with
only household ingredients (a C compiler and the m4 macro processor) and
comes with all the knickknackery and whatnots:

+ GNU-style long options (`--blah`)
+ condensable short options (`-xab` for `-x -a -b`)
+ optional arguments to long and short options (--foo[=BAR])
+ multiple occurrence of options (-vvv)
+ does *not* depend on libc's getopt() nor getopt_long()
+ [BSD 3-clause licence][4]

And getting started is as easy as *munching* cake -- let yuck do the
actual baking for you:  Just feed it the `--help` output you'd like to
see and yuck will happily try and generate a parser from it.


That all? I need more highlights
--------------------------------

yuck can also generate parsers for umbrella tools, i.e. tools that take
a command as argument (think git(1), ip(8), etc.).

yuck has no exotic build time or run time dependencies, a C99 compiler and
the m4 macro processor is enough.

yuck can be used in other projects by copying 4 files and setting up
a simple Makefile rule.

yuck can generate man pages based on the definition files (the --help
output), much like [help2man](http://www.gnu.org/s/help2man/).

yuck can automatically determine (and make use of) version numbers in
git controlled projects.


But why?
--------
There's [AutoOpts](http://autogen.sourceforge.net/autoopts.html),
there's [gengetopt](http://www.gnu.org/software/gengetopt/), lately even
glibc takes on arg parsing (see their argp section in the manual); makes
you wonder how we dare create yet another thing for something as simple
as command line argument parsing.

Well, the killer feature, as we see it, is yuck's approach to specifying
the parser in question.  You expect your users to grasp your `--help`
output?  Well, there you go, if your users can understand it so can you!
Just type up what you'd like to see in your `--help` output and yuck
will generate a parser that does exactly that.


No, the other why?
------------------
yuck has been crafted by a heavy gengetopt user, so both the procedure
and the handling is quite similar to the ggo workflow.

While gengetopt does a great job most of the time, it becomes annoying
in some corner cases, is largely undermaintained, counts on libc for the
actual getopt()'ing, is GPL licensed but first and foremost it is
certainly not the right tool for the job if the job is parsing options
for umbrella programs.


And what about docopt?
----------------------
While [docopt](http://docopt.org/) is based on essentially the same idea
as yuck, its grammar is formal and doesn't allow for descriptive texts.
Also, docopt's C parser (yuck's primary target) is not fully functional.

However, if you're currently using docopt and you feel comfortable with
it, there's no need to switch to yuck.


Got an example?
---------------
Consider the following .yuck file:

    Usage: xmpl
    Shows off yuck.

      -x, --extra        Produce some extra bling.
      -o, --output=FILE  Output bling to file.

Process with:

    $ yuck gen xmpl.yuck > xmpl.yucc

Then include in your `xmpl.c`:

    #include <stdio.h>
    #include "xmpl.yucc"

    int main(int argc, char *argv[])
    {
            yuck_t argp[1];
    
            yuck_parse(argp, argc, argv);
    
            if (argp->extra_flag) {
                    puts("BLING BLING!");
            }
    
            yuck_free(argp);
            return 0;
    }

And that's it.  Some example calls:

    $ xmpl --help
    Usage: xmpl [OPTION]...

    Shows off yuck.

      -h, --help            display this help and exit
      -V, --version         output version information and exit
      -x, --extra           Produce some extra bling.
      -o, --output=FILE     Output bling to file.

    $ xmpl -x
    BLING BLING!
    $

More details, please
--------------------
The example above results in an auxiliary struct:

    struct yuck_s {
            enum yuck_cmds_e cmd;
    
            /* left-over arguments, the command string is never a part of this */
            size_t nargs;
            char *const *args;
    
            /* slots common to all commands */
    
            /* help is handled automatically */
            /* version is handled automatically */
            unsigned int extra_flag;
            const char *output_arg;
    };

which is filled in when `yuck_parse()` is run.  As there are no
subcommands defined this struct will directly be typedef'd to `yuck_t`
and the `cmd` slot at the top will always hold `YUCK_NOCMD`.

Every occurrence of `-x` or `--extra` on the command line will increase
the count in `extra_flag`, yuck does not distinguish between optional
flags (to occur at most once), flags to occur exactly once, or flags
that can occur multiple times.

Same goes for every occurrence of `-o` or `--output`, however, the
pointer in `output_arg` will point to the last occurrence on the
commandline.

Left-over positional arguments will be counted in `nargs` and collected
into `args`.  It is never an error to pass in positional arguments.  It
is up to the caller of `yuck_parse()` to check the yuck_t representation
of the command line for integrity.

In a similar fashion, yuck's only types are options with arguments
(which are mapped to const char* or const char** in case of multi-args)
and flags (mapped to unsigned int, representing the number of occurences
on the command line).  Again, it is up to the postprocessing code to
interpret arguments suitably, e.g. convert integer strings to integers,
or constrain a HOSTNAME argument to its legal characters, etc.

All const char* objects point straight to members of `argv`, i.e. they
are not `strdup()`ed.  Changing strings in argv will therefore change
the strings in the yuck_t representation also, and vice versa (after
by-passing the const qualifier).


So what about subcommands?
--------------------------
yuck's command-line interface is generated by yuck itself, so for an
hands-on example have a look there.

Subcommands are specified through extra usage clauses:

    Usage: xmpl
    Shows off yuck.
    
      -x, --extra        Produce some extra bling.
      -o, --output=FILE  Output bling to file.
    
    Usage: xmpl turbo [FILE]...
    Run xmpl in turbo mode
    
      -x, --extra-turbo  Use more turbos than normal.

Again, generate a C parser:

    $ yuck gen xmpl-subcommands.yuck

The auxiliaries generated will now look like:

    typedef union yuck_u yuck_t;
    
    /* convenience structure for `turbo' */
    struct yuck_cmd_turbo_s {
            enum yuck_cmds_e cmd;
    
            /* left-over arguments, the command string is never a part of this */
            size_t nargs;
            char *const *args;
    
            /* help is handled automatically */
            /* version is handled automatically */
            unsigned int extra_flag;
            const char *output_arg;
    
            unsigned int extra_turbo_flag;
    };
    
    union yuck_u {
            /* generic struct */
            struct {
                    enum yuck_cmds_e cmd;
    
                    /* left-over arguments,
                     * the command string is never a part of this */
                    size_t nargs;
                    char *const *args;
    
                    /* slots common to all commands */
                    /* help is handled automatically */
                    /* version is handled automatically */
                    unsigned int extra_flag;
                    const char *output_arg;
            };
    
            /* depending on CMD at most one of the following structs is filled in
             * if CMD is YUCK_NONE no slots of this union must be accessed */
            struct yuck_cmd_turbo_s turbo;
    };

and when `yuck_parse()` is run, provided the `turbo` command is given,
the struct yuck_cmd_turbo_s object will be filled in, `cmd` will be set
to `XMPL_CMD_TURBO` in such case.  If no command is given the generic
struct at the top of the union will be filled in and `cmd` is set to
`XMPL_CMD_NONE`.

The structs are carefully generated in a way that allows you to simple
cast a `yuck_t*` to a `struct yuck_cmd_turbo_s*`.


And now handing the `--help` output over to help2man again?!
------------------------------------------------------------
Nope.  Of course not.  If we can create something as formal and
definitive as a command-line option parser, we sure as hell can create
something sloppy and informal as a man page (that is, no offence, for
human eyes only anyway).

yuck comes with a template `yuck.man.m4` for that purpose which is
materialised through the `genman` command:

    $ yuck genman xmpl-subcommands.yuck

would produce [xmpl-subcommands.man](xmpl-subcommands.html) (which here
for obvious reasons has been run through man2html first).


This is all superfluous and utter rubbish because ...
-----------------------------------------------------
Don't let me stop you there.  I'm all ears for feature requests, patches,
criticism and insults, oh, and death threats, of course.

Best to use the [bug tracker][1], or drop me an [email][2], or just put
a huuuge graffiti [on my house][3].

  [1]: https://github.com/hroptatyr/yuck/issues
  [2]: mailto:devel+yuck@fresse.org
  [3]: http://www.fresse.org/yuck/my-house.jpg
  [4]: http://opensource.org/licenses/BSD-3-Clause

<!--
  Local variables:
  mode: auto-fill
  fill-column: 72
  filladapt-mode: t
  End:
-->
