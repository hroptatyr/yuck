yuck
====
### Your Umbrella Command Kit

yuck is a bog-standard command line option parser for C that works with
only household ingredients (a C compiler and the m4 macro processor) and
comes with all the knickknackery and whatnots:
+ GNU-style long options (`--blah`)
+ condensable short options (`-xab` for `-x -a -b`)
+ optional arguments to long and short options (--foo[=BAR])
+ multiple occurrence of options (-vvv)

yuck can also generate parsers for umbrella tools, i.e. tools that take
a command as argument (think git(1), ip(8), etc.).

yuck comes along with a DSL written in m4 and a simple parser that takes
free text that looks like the `--help` output and returns m4-directives.

yuck has no build or run time dependencies except for a C99 compiler and
the m4 macro processor.

yuck can be used in other projects by copying 4 files and setting up
a simple Makefile rule.


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


Example
-------
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
            struct yuck_s argp[1];
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
