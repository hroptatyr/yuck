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
	define([YUCK_UMB.POSARG], [$2])
])

define([yuck_set_umbrella_desc], [dnl
	pushdef([umb], make_c_ident([$1]))
	pushdef([desc], [$2])

	define([YUCK_UMB.]defn([umb])[.desc], defn([desc]))

	popdef([umb])
	popdef([desc])
])

## yuck_add_command([CMD], [[POSARG]])
define([yuck_add_command], [dnl
	pushdef([cmd], make_c_ident([$1]))
	append_nene([YUCK_CMD], defn([cmd]), [,])
	define([YUCK_STR.]defn([cmd]), [$1])
	define([YUCK_POSARG.]defn([cmd]), [$2])
	popdef([cmd])
])

define([yuck_set_command_desc], [dnl
	pushdef([cmd], make_c_ident([$1]))
	pushdef([desc], [$2])

	define([YUCK_CMD.]defn([cmd])[.desc], defn([desc]))

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

	ifdef([YUCK.]cmd[.]ident[.canon], [], [dnl
		## process only if new
		append_ne([YUCK.]defn([cmd])[.S], defn([short]), [,])
		append_ne([YUCK.]defn([cmd])[.L], defn([long]), [,])
		append_ne([YUCK.]defn([cmd])[.I], defn([ident]), [,])

		define([YUCK.]defn([cmd])[.]defn([short])[.canon], defn([ident]))
		define([YUCK.]defn([cmd])[.]defn([long])[.canon], defn([ident]))
		define([YUCK.]defn([cmd])[.]defn([ident])[.canon], defn([ident]))
		define([YUCK.]defn([cmd])[.]defn([short])[.type], defn([type]))
		define([YUCK.]defn([cmd])[.]defn([long])[.type], defn([type]))
		define([YUCK.]defn([cmd])[.]defn([ident])[.type], defn([type]))

		## reverse maps
		define([YUCK.]defn([cmd])[.]defn([ident])[.short], defn([short]))
		define([YUCK.]defn([cmd])[.]defn([ident])[.long], defn([long]))
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

	define([YUCK.]defn([cmd])[.]defn([ident])[.desc], defn([desc]))

	popdef([ident])
	popdef([short])
	popdef([long])
	popdef([cmd])
	popdef([desc])
])


## helpers for the m4c and m4h

## yuck_canon([opt], [[cmd]])
define([yuck_canon], [defn([YUCK.$2.$1.canon])])

## yuck_type([opt], [[cmd]])
define([yuck_type], [first(defn([YUCK.$2.$1.type]))])

## yuck_arg_name([opt], [[cmd]])
define([yuck_arg_name], [second(defn([YUCK.$2.$1.type]))])

## yuck_arg_suf([opt], [[cmd]])
define([yuck_arg_suf], [thirds(defn([YUCK.$2.$1.type]))])

## yuck_slot_identifier([option], [[cmd]])
define([yuck_slot_identifier], [dnl
pushdef([canon], yuck_canon([$1], [$2]))dnl
pushdef([type], yuck_type([$1], [$2]))dnl
dnl
defn([canon])[_]defn([type])[]dnl
yuck_iftype([$1], [$2], [arg,mul], [s], [arg,mul,opt], [s])[]dnl
dnl
popdef([canon])dnl
popdef([type])dnl
])

## yuck_cnt_slot([option], [[cmd]])
define([yuck_cnt_slot], [dnl
pushdef([idn], yuck_canon([$1], [$2])[_nargs])dnl
pushdef([res], ifelse([$2], [], defn([idn]), [$2.]defn([idn])))dnl
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
pushdef([type], yuck_type([$1], [$2]))dnl
pushdef([tsuf], yuck_arg_suf([$1], [$2]))dnl
append_ne([type], defn([tsuf]), [,])[]dnl
[]ifelse(_splice(defn([type]), shift(shift($@))))[]dnl
popdef([tsuf])dnl
popdef([type])dnl
])

## yuck_umbcmds(), umbrella + commands
define([yuck_umbcmds], [ifdef([YUCK_CMD], [[,]defn([YUCK_CMD])], dquote([[]]))])

## yuck_cmds(), just the commands
define([yuck_cmds], [defn([YUCK_CMD])])

## yuck_cmd
define([yuck_cmd], [YUCK_UMC[_]ifelse([$1], [], [NONE], [$1])])

## yuck_cmd_string
define([yuck_cmd_string], [defn([YUCK_STR.]$1)])

## yuck_cmd_posarg
define([yuck_cmd_posarg], [defn([YUCK_POSARG.]$1)])

## yuck_umb_desc([[umb]]) getter for the umbrella description
define([yuck_umb_desc], [defn([YUCK_UMB.]ifelse([$1], [], defn([YUCK_UMB]), [$1])[.desc])])

## yuck_cmd_desc([cmd]) getter for the command description
define([yuck_cmd_desc], [defn([YUCK_CMD.$1.desc])])

## yuck_shorts([cmd])
define([yuck_shorts], [defn([YUCK.]$1[.S])])

## yuck_longs([cmd])
define([yuck_longs], [defn([YUCK.]$1[.L])])

## yuck_idents([cmd])
define([yuck_idents], [defn([YUCK.]$1[.I])])

## yuck_short([ident], [[cmd]])
define([yuck_short], [defn([YUCK.]$2[.]$1[.short])])

## yuck_long([ident], [[cmd]])
define([yuck_long], [defn([YUCK.]$2[.]$1[.long])])

## yuck_option_desc([ident], [[cmd]])
define([yuck_option_desc], [defn([YUCK.]$2[.]$1[.desc])])

## yuck_option_help_lhs([ident], [[cmd]])
define([yuck_option_help_lhs], [dnl
pushdef([s], yuck_short([$1], [$2]))dnl
pushdef([l], yuck_long([$1], [$2]))dnl
pushdef([an], yuck_arg_name([$1], [$2]))dnl
pushdef([as], yuck_arg_suf([$1], [$2]))dnl
pushdef([optp], ifelse(defn([as]), [], [0], defn([as]), [mul], [0], [1]))dnl
pushdef([oo], ifelse(optp, [0], [], LBRACK))dnl
pushdef([oc], ifelse(optp, [0], [], RBRACK))dnl
pushdef([ds], ifelse(defn([s]), [], [  ],
	[-]defn([s])[]ifelse(defn([an]), [], [], l, [],
	[ ]defn([oo])defn([an])defn([oc]))))dnl
pushdef([dl], ifelse(defn([l]), [], [],
	[--]defn([l])[]ifelse(defn([an]), [], [],
	defn([oo])[=]defn([an])defn([oc]))))dnl
[  ]ds[]ifelse(defn([s]), [], [  ], defn([l]), [], [], [[, ]])defn([dl])[]dnl
popdef([s])dnl
popdef([l])dnl
popdef([an])dnl
popdef([ds])dnl
popdef([dl])dnl
])

## yuck_option_help_line([ident], [[cmd]])
define([yuck_option_help_line], [dnl
pushdef([lhs], yuck_option_help_lhs([$1], [$2]))dnl
pushdef([desc], patsubst([yuck_option_desc([$1], [$2])], [
], [
                        ]))dnl
ifelse(eval(len(defn([lhs])) >= 24), [0], [dnl
format([[[%-22s  %s]]], defn([lhs]), defn([desc]))], [dnl
defn([lhs])[]
                        defn([desc])[]dnl
])
popdef([lhs])dnl
popdef([desc])dnl
])

## \ -> \\, " -> \" and literal \n -> \\n\\
define([_yuck_C_literal], [dnl
patsubst(patsubst(patsubst([[[[$1]]]], [\\], [\\\\]), ["], [\\"]), [
], [\\n\\
])dnl
])
define([yuck_C_literal], [dnl
translit(_yuck_C_literal([$1]), LBRACK[]RBRACK, [[]])dnl
])dnl


## coroutine stuff
define([yield], [goto $1; back_from_$1:])
define([coroutine], [define([this_coru], [$1])$1:])
define([resume], [goto back_from_[]this_coru])
define([resume_at], [goto $1])
define([quit], [goto out])

divert[]dnl
changequote`'dnl
