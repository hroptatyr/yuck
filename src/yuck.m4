changequote([,])dnl
divert([-1])

## this is a little domain language for the yuck processor

## our own version of [ and ] (the quoting characters)
## must be in sync with yuck.c
define([LBRACK], format([%c], 2))
define([RBRACK], format([%c], 3))

# foreachq(x, [item_1, item_2, ..., item_n], stmt)
#   quoted list, alternate improved version
define([foreachq], [ifelse([$2], [], [],
	[pushdef([$1])_$0([$1], [$3], [], $2)popdef([$1])])])
define([_foreachq], [ifelse([$#], [3], [],
	[define([$1], [$4])$2[]$0([$1], [$2],
		shift(shift(shift($@))))])])

define([append], [define([$1], ifdef([$1], [defn([$1])[$3]])[$2])])
## like append, but append only non-empty arguments
define([append_ne], [ifelse([$2], [], [], [append([$1], [$2], [$3])])])
## like append_ne, but append only non-existing arguments
define([append_nene], [ifelse(index([$3]defn([$1])[$3], [$3$2$3]), [-1],
	[append_ne([$1], [$2], [$3])])])

define([appendq], [define([$1], ifdef([$1], [defn([$1])[$3]])dquote([$2]))])
## like appendq, but append only non-empty arguments
define([appendq_ne], [ifelse([$2], [], [], [append([$1], dquote([$2]), [$3])])])
## like appendq_ne, but append only non-existing arguments
define([appendq_nene], [ifelse(index([$3]defn([$1])[$3], [$3]dquote($2)[$3]), [-1],
	[appendq_ne([$1], [$2], [$3])])])

define([first_nonnil], [ifelse([$#], [0], [], [$1], [],
	[first_nonnil(shift($@))], [], [], [$1])])
define([first], [_first($*)])
define([_first], [$1])
define([second], [_second($*)])
define([_second], [$2])
define([thirds], [_thirds($*)])
define([_thirds], [quote(shift(shift($@)))])

define([quote], [ifelse([$#], [0], [], [[$*]])])
define([dquote], [[$@]])
define([equote], [dquote($*)])

define([_splice], [ifelse(eval([$#] > [3]), [0], [[$1], [$2], [$3]], [[$1], [$2], [$3], _splice([$1], shift(shift(shift($@))))])])

define([cond], [ifelse([$#], [0], [], [$#], [1], [$1], [_$0($@)])])
define([_cond], [dnl
ifelse([$1], [$2], [$3],
	[$#], [3], [],
	[$#], [4], [$4],
	[$0([$1], shift(shift(shift($@))))])])

define([make_c_ident], [dnl
translit([$1], [!"#$%&'()*+,-./:;<=>?@[\]^`{|}~],dnl "
	[_________________________________])[]dnl
])

define([downcase], [dnl
translit([$1], [ABCDEFGHIJKLMNOPQRSTUVWXYZ], [abcdefghijklmnopqrstuvwxyz])[]dnl
])

define([upcase], [dnl
translit([$1], [abcdefghijklmnopqrstuvwxyz], [ABCDEFGHIJKLMNOPQRSTUVWXYZ])[]dnl
])

## select a diversion, clearing all other diversions
define([select_divert], [divert[]undivert($1)[]divert(-1)[]undivert[]divert(0)])


define([yuck_set_version], [dnl
	define([YUCK_VER], [$1])
])

## yuck_set_umbrella([UMB], [[POSARG]])
define([yuck_set_umbrella], [dnl
	define([YUCK_UMB], [$1])
	define([YUCK_UMC], make_c_ident([$1]))
	define([YUCK_UMB_POSARG], [$2])
])

define([yuck_set_umbrella_desc], [dnl
	pushdef([umb], make_c_ident([$1]))
	pushdef([desc], [$2])

	define([YUCK_UMB_]defn([umb])[_desc], defn([desc]))

	popdef([umb])
	popdef([desc])
])

## yuck_add_command([CMD], [[POSARG]])
define([yuck_add_command], [dnl
	pushdef([cmd], make_c_ident([$1]))
	append_nene([YUCK_CMD], defn([cmd]), [,])
	define([YUCK_STR_]defn([cmd]), [$1])
	define([YUCK_POSARG_]defn([cmd]), [$2])
	popdef([cmd])
])

define([yuck_set_command_desc], [dnl
	pushdef([cmd], make_c_ident([$1]))
	pushdef([desc], [$2])

	define([YUCK_CMD_]defn([cmd])[_desc], defn([desc]))

	popdef([cmd])
	popdef([desc])
])

## yuck_add_option(short, long, type, [CMD])
define([yuck_add_option], [dnl
	## quote the elements of the type arg first
	## before any possible expansion is in scope
	pushdef([type], equote([$3]))
	pushdef([short], [$1])
	pushdef([long], [$2])
	pushdef([cmd], make_c_ident([$4]))

	pushdef([ident], ifelse(defn([long]), [],
		ifelse(defn([short]), [],
			[define([cnt], ifdef([cnt], [incr(cnt)], [0]))[s]cnt],
			[dash]defn([short])), make_c_ident(defn([long]))))

	ifdef([YUCK_]cmd[_]ident[_canon], [], [dnl
		## process only if new
		appendq_ne([YUCK_]defn([cmd])[_S], defn([short]), [,])
		appendq_ne([YUCK_]defn([cmd])[_L], defn([long]), [,])
		appendq_ne([YUCK_]defn([cmd])[_I], defn([ident]), [,])

		define([YUCK_]defn([cmd])[_]defn([short])[_canon], defn([ident]))
		define([YUCK_]defn([cmd])[_]defn([long])[_canon], defn([ident]))
		define([YUCK_]defn([cmd])[_]defn([ident])[_canon], defn([ident]))
		define([YUCK_]defn([cmd])[_]defn([short])[_type], defn([type]))
		define([YUCK_]defn([cmd])[_]defn([long])[_type], defn([type]))
		define([YUCK_]defn([cmd])[_]defn([ident])[_type], defn([type]))

		## reverse maps
		define([YUCK_]defn([cmd])[_]defn([ident])[_short], defn([short]))
		define([YUCK_]defn([cmd])[_]defn([ident])[_long], defn([long]))
	])

	popdef([ident])
	popdef([cmd])
	popdef([type])
	popdef([long])
	popdef([short])
])

define([yuck_set_option_desc], [dnl
	pushdef([short], [$1])
	pushdef([long], [$2])
	pushdef([cmd], make_c_ident([$3]))
	pushdef([desc], [$4])

	pushdef([ident], ifelse(defn([long]), [],
		ifelse(defn([short]), [],
			[define([dcnt], ifdef([dcnt], [incr(dcnt)], [0]))[s]dcnt],
			[dash]defn([short])), make_c_ident(defn([long]))))

	define([YUCK_]defn([cmd])[_]defn([ident])[_desc], defn([desc]))

	popdef([ident])
	popdef([short])
	popdef([long])
	popdef([cmd])
	popdef([desc])
])


## helpers for the m4c and m4h

## yuck_canon([opt], [[cmd]])
define([yuck_canon], [defn([YUCK_$2_$1_canon])])

## yuck_option_type([opt], [[cmd]])
define([yuck_option_type], [defn([YUCK_$2_$1_type])])

## yuck_type([type-spec])
define([yuck_type], [first([$1])])

## yuck_type_name([type-spec])
define([yuck_type_name], [second([$1])])

## yuck_type_sufx([type-spec])
define([yuck_type_sufx], [thirds([$1])])

## yuck_slot_identifier([option], [[cmd]])
define([yuck_slot_identifier], [dnl
pushdef([canon], yuck_canon([$1], [$2]))dnl
pushdef([type], yuck_option_type([$1], [$2]))dnl
dnl
defn([canon])[_]yuck_type(defn([type]))[]dnl
cond(yuck_type_sufx(defn([type])), [mul], [s], [mul,opt], [s])[]dnl
dnl
popdef([canon])dnl
popdef([type])dnl
])

## yuck_cnt_slot([option], [[cmd]])
define([yuck_cnt_slot], [dnl
pushdef([idn], yuck_canon([$1], [$2])[_nargs])dnl
pushdef([res], ifelse([$2], [], defn([idn]), [$2_]defn([idn])))dnl
yuck_iftype([$1], [$2],
	[arg,mul], [defn([res])],
	[arg,mul,opt], [defn([res])],
)dnl
popdef([idn])dnl
popdef([res])dnl
])

## yuck_slot([option], [[cmd]])
define([yuck_slot], [dnl
pushdef([idn], yuck_slot_identifier([$1], [$2]))dnl
dnl
ifelse([$2], [], defn([idn]), [$2.]defn([idn]))[]dnl
dnl
popdef([idn])dnl
])

## yuck_iftype([opt], [cmd], [type], [body], [[type], [body]]...)
define([yuck_iftype], [dnl
pushdef([type], yuck_option_type([$1], [$2]))dnl
pushdef([tsuf], yuck_type_sufx(defn([type])))dnl
pushdef([res], yuck_type(defn([type])))dnl
append_ne([res], defn([tsuf]), [,])[]dnl
[]ifelse(_splice(defn([res]), shift(shift($@))))[]dnl
popdef([tsuf])dnl
popdef([type])dnl
popdef([res])dnl
])

## yuck_umbcmds(), umbrella + commands
define([yuck_umbcmds], [ifdef([YUCK_CMD], [[,]defn([YUCK_CMD])], dquote([[]]))])

## yuck_cmds(), just the commands
define([yuck_cmds], [defn([YUCK_CMD])])

## yuck_cmd
define([yuck_cmd], [upcase(defn([YUCK_UMC]))[_CMD_]ifelse([$1], [], [NONE], [upcase([$1])])])

## yuck_cmd_string
define([yuck_cmd_string], [defn([YUCK_STR_]$1)])

## yuck_cmd_posarg
define([yuck_cmd_posarg], [defn([YUCK_POSARG_]$1)])

## yuck_umb_desc([[umb]]) getter for the umbrella description
define([yuck_umb_desc], [defn([YUCK_UMB_]ifelse([$1], [], defn([YUCK_UMB]), [$1])[_desc])])

## yuck_cmd_desc([cmd]) getter for the command description
define([yuck_cmd_desc], [defn([YUCK_CMD_$1_desc])])

## yuck_shorts([cmd])
define([yuck_shorts], [defn([YUCK_]$1[_S])])

## yuck_longs([cmd])
define([yuck_longs], [defn([YUCK_]$1[_L])])

## yuck_idents([cmd])
define([yuck_idents], [defn([YUCK_]$1[_I])])

## yuck_short([ident], [[cmd]])
define([yuck_short], [defn([YUCK_]$2[_]$1[_short])])

## yuck_long([ident], [[cmd]])
define([yuck_long], [defn([YUCK_]$2[_]$1[_long])])

## yuck_option_desc([ident], [[cmd]])
define([yuck_option_desc], [defn([YUCK_]$2[_]$1[_desc])])

## yuck_option_help_lhs([ident], [[cmd]])
define([yuck_option_help_lhs], [dnl
pushdef([s], [yuck_short([$1], [$2])])dnl
pushdef([l], [yuck_long([$1], [$2])])dnl
pushdef([an], [yuck_arg_name([$1], [$2])])dnl
pushdef([as], [yuck_arg_suf([$1], [$2])])dnl
pushdef([optp], [ifelse(quote(as), [], [0], quote(as), [mul], [0], [1])])dnl
pushdef([oo], ifelse(optp, [0], [], LBRACK))dnl
pushdef([oc], ifelse(optp, [0], [], RBRACK))dnl
pushdef([ds], [ifelse(quote(s), [], [  ],
	[-s]ifelse(quote(an), [], [], quote(l), [], [ oo[]an[]oc]))])dnl
pushdef([dl], [ifelse(quote(l), [], [],
	[--l]ifelse(quote(an), [], [], [oo[]=[]an[]oc]))])dnl
[  ]ds[]ifelse(quote(s), [], [  ], quote(l), [], [], [[, ]])dl[]dnl
popdef([s])dnl
popdef([l])dnl
popdef([an])dnl
popdef([ds])dnl
popdef([dl])dnl
])

## yuck_option_help_line([ident], [[cmd]])
define([yuck_option_help_line], [dnl
pushdef([lhs], yuck_option_help_lhs([$1], [$2]))dnl
pushdef([desc], patsubst(dquote(yuck_option_desc([$1], [$2])), [
], [
                        ]))dnl
ifelse(eval(len(defn([lhs])) >= 24), [0], [dnl
format([[[%-22s  %s]]], defn([lhs]), defn([desc]))], [dnl
format([[[%s
                        %s]]], defn([lhs]), defn([desc]))[]dnl
])
popdef([lhs])dnl
popdef([desc])dnl
])

## \ -> \\, " -> \" and literal \n -> \\n\\
define([_yuck_C_literal], [dnl
patsubst(patsubst(patsubst([[[[$1]]]], [\\], [\\\\]), ["], [\\"]), [
], [\\n\\
])[]dnl
])
define([yuck_C_literal], [dnl
translit(_yuck_C_literal([$1]), LBRACK[]RBRACK, [[]])[]dnl
])dnl


## coroutine stuff
define([yield], [goto $1; back_from_$1:])
define([coroutine], [define([this_coru], [$1])$1:])
define([resume], [goto back_from_[]this_coru])
define([resume_at], [goto $1])
define([quit], [goto out])

divert[]dnl
changequote`'dnl
