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

## yuck_set_umbrella([ident], [umbrella], [[posarg]])
define([yuck_set_umbrella], [dnl
	define([YUCK_CURRENT_UMB], [$1])
	define([YUCK_UMB], [$2])
	define([YUCK_UMB_POSARG], [$3])
])

## yuck_set_umbrella_desc([ident], [desc])
define([yuck_set_umbrella_desc], [dnl
	define([YUCK_UMB_$1_desc], [$2])
])

## yuck_add_command([ident], [command], [[posarg]])
define([yuck_add_command], [dnl
	define([YUCK_CURRENT_CMD], [$1])
	append_nene([YUCK_CMD], [$1], [,])
	define([YUCK_STR_$1], [$2])
	define([YUCK_POSARG_$1], [$3])
])

## yuck_set_command_desc([ident], [desc])
define([yuck_set_command_desc], [dnl
	define([YUCK_CMD_$1_desc], [$2])
])

## yuck_add_option([ident], [short], [long], [type])
define([yuck_add_option], [dnl
	## quote the elements of the type arg first
	## before any possible expansion is in scope
	pushdef([type], equote([$4]))
	pushdef([ident], [$1])
	pushdef([cmd], defn([YUCK_CURRENT_CMD]))

	ifelse([$2], [], [],
		index([0123456789], [$2]), [-1], [],
		[dnl else
		define([YUCK_SHORTS_HAVE_NUMERALS], [1])
	])

	ifdef([YUCK_]defn([cmd])[_]defn([ident])[_canon], [], [dnl
		## process only if new
		appendq_ne([YUCK_]defn([cmd])[_I], defn([ident]), [,])

		## forward maps
		define([YUCK_]defn([cmd])[_]defn([ident])[_canon], defn([ident]))
		define([YUCK_]defn([cmd])[_]defn([ident])[_type], defn([type]))

		## reverse maps
		define([YUCK_]defn([cmd])[_]defn([ident])[_short], [$2])
		define([YUCK_]defn([cmd])[_]defn([ident])[_long], [$3])
	])

	popdef([ident])
	popdef([cmd])
	popdef([type])
])

## yuck_set_option_desc([ident], [desc])
define([yuck_set_option_desc], [dnl
	define([YUCK_]defn([YUCK_CURRENT_CMD])[_$1_desc], [$2])
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

## yuck_cmd([command])
define([yuck_cmd], [upcase(defn([YUCK_CURRENT_UMB]))[_CMD_]ifelse([$1], [], [NONE], [upcase([$1])])])

## yuck_cmd_string
define([yuck_cmd_string], [defn([YUCK_STR_]$1)])

## yuck_cmd_posarg
define([yuck_cmd_posarg], [defn([YUCK_POSARG_]$1)])

## yuck_umb_desc([[umb]]) getter for the umbrella description
define([yuck_umb_desc], [defn([YUCK_UMB_]ifelse([$1], [], defn([YUCK_UMB]), [$1])[_desc])])

## yuck_cmd_desc([cmd]) getter for the command description
define([yuck_cmd_desc], [defn([YUCK_CMD_$1_desc])])

## yuck_idents([cmd])
define([yuck_idents], [defn([YUCK_$1_I])])

## yuck_short([ident], [[cmd]])
define([yuck_short], [defn([YUCK_$2_$1_short])])

## yuck_long([ident], [[cmd]])
define([yuck_long], [defn([YUCK_$2_$1_long])])

## yuck_option_desc([ident], [[cmd]])
define([yuck_option_desc], [defn([YUCK_$2_$1_desc])])

## yuck_option_help_lhs([ident], [[cmd]])
define([yuck_option_help_lhs], [dnl
pushdef([s], [yuck_short([$1], [$2])])dnl
pushdef([l], [yuck_long([$1], [$2])])dnl
pushdef([type], yuck_option_type([$1], [$2]))dnl
pushdef([an], [yuck_type_name(defn([type]))])dnl
pushdef([as], [yuck_type_sufx(defn([type]))])dnl
pushdef([optp], [ifelse(quote(as), [], [0], quote(as), [mul], [0], [1])])dnl
pushdef([oo], ifelse(optp, [0], [], LBRACK))dnl
pushdef([oc], ifelse(optp, [0], [], RBRACK))dnl
pushdef([ds], [ifelse(quote(s), [], [  ],
	[-s]ifelse(an, [], [], quote(l), [], [ oo[]an[]oc]))])dnl
pushdef([dl], [ifelse(quote(l), [], [],
	[--l]ifelse(an, [], [], [oo[]=[]an[]oc]))])dnl
[  ]ds[]ifelse(quote(s), [], [  ], quote(l), [], [], [[, ]])dl[]dnl
popdef([s])dnl
popdef([l])dnl
popdef([type])dnl
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
