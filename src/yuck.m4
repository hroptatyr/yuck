changequote([,])dnl
divert([-1])

## this is a little domain language for the yuck processor

# foreachq(x, [item_1, item_2, ..., item_n], stmt)
#   quoted list, alternate improved version
define([foreachq], [ifelse([$2], [], [],
	[pushdef([$1])_$0([$1], [$3], [], $2)popdef([$1])])])
define([_foreachq], [ifelse([$#], [3], [],
	[define([$1], [$4])$2[]$0([$1], [$2],
		shift(shift(shift($@))))])])

define([append], [dnl
	define([$1], ifdef([$1], [defn([$1])[$3]])[$2])
])
define([append_ne], [dnl
	## like append, but append only non-empty arguments
	ifelse([$2], [], [], [append([$1], [$2], [$3])])
])
define([append_nene], [dnl
	## like append_ne, but append only non-existing arguments
	ifelse(index([$3]defn([$1])[$3], [$3$2$3]), [-1], [append_ne([$1], [$2], [$3])])
])

define([first_nonnil], [dnl
	ifelse([$#], [0], [], [$1], [], [first_nonnil(shift($@))], [], [], [$1])
])

define([make_c_ident], [dnl
translit([$1], [!"#$%&'()*+,-./:;<=>?@[\]^`{|}~],dnl "
	[_________________________________])[]dnl
])

## select a diversion, clearing all other diversions
define([select_divert], [divert[]undivert($1)[]divert(-1)[]undivert[]divert(0)])


define([yuck_set_umbrella], [dnl
	define([YUCK_UMB], [$1])
	define([YUCK_UMC], make_c_ident([$1]))
])
define([yuck_set_version], [dnl
	define([YUCK_VER], [$1])
])
define([yuck_add_command], [dnl
	pushdef([cmd], make_c_ident([$1]))
	pushdef([str], [ifelse([$2], [], [$1], cmd)])
	append_nene([YUCK_CMD], cmd, [,])
	define([YUCK_STR.]cmd, str)
	popdef([str])
	popdef([cmd])
])

## yuck_add_option(short, long, type, [CMD])
define([yuck_add_option], [dnl
	pushdef([short], [$1])
	pushdef([long], [$2])
	pushdef([type], [$3])
	pushdef([cmd], make_c_ident([$4]))

	pushdef([ident], ifelse(long, [], ifelse(short, [], [define([cnt], ifdef([cnt], [incr(cnt)], [0]))[s]cnt], [dash]short), make_c_ident(long)))

	ifdef([YUCK.]cmd[.]ident[.canon], [], [dnl
		## process only if new
		append_ne([YUCK.]cmd[.S], short, [,])
		append_ne([YUCK.]cmd[.L], long, [,])
		append_ne([YUCK.]cmd[.I], ident, [,])

		define([YUCK.]cmd[.]short[.canon], ident)
		define([YUCK.]cmd[.]long[.canon], ident)
		define([YUCK.]cmd[.]ident[.canon], ident)
		define([YUCK.]cmd[.]short[.type], type)
		define([YUCK.]cmd[.]long[.type], type)
		define([YUCK.]cmd[.]ident[.type], type)
	])

	popdef([ident])
	popdef([slot])
	popdef([cmd])
	popdef([type])
	popdef([long])
	popdef([short])
])

## helpers for the m4c and m4h

## yuck_canon([opt], [[cmd]])
define([yuck_canon], [dnl
pushdef([opt], [$1])dnl
pushdef([cmd], [$2])dnl
defn([YUCK.]cmd[.]opt[.canon])dnl
popdef([cmd])dnl
popdef([opt])dnl
])

## yuck_slot_decl([option], [[cmd]])
define([yuck_slot_decl], [dnl
pushdef([opt], [$1])dnl
pushdef([cmd], [$2])dnl
pushdef([type], defn([YUCK.]cmd[.]opt[.type]))dnl
dnl
pushdef([ctype],
	ifelse(
		type, [flag], [unsigned int ],
		type, [arg], [const char *],
		type, [marg], [const char **],
		type, [auto], [unsigned int ],
		[], [], [void ]))dnl
pushdef([cpost],
	ifelse(type, [auto], [[[[0U]]]]))dnl
dnl
ctype[]yuck_slot_identifier(opt, cmd)[]cpost[]dnl
dnl
popdef([cpost])dnl
popdef([ctype])dnl
popdef([type])dnl
popdef([cmd])dnl
popdef([opt])dnl
])

## yuck_slot_identifier([option], [[cmd]])
define([yuck_slot_identifier], [dnl
pushdef([opt], [$1])dnl
pushdef([cmd], [$2])dnl
pushdef([canon], defn([YUCK.]cmd[.]opt[.canon]))dnl
pushdef([type], defn([YUCK.]cmd[.]opt[.type]))dnl
dnl
canon[_]type[]dnl
dnl
popdef([canon])dnl
popdef([type])dnl
popdef([cmd])dnl
popdef([opt])dnl
])

## yuck_slot([option], [[cmd]])
define([yuck_slot], [dnl
pushdef([opt], [$1])dnl
pushdef([cmd], [$2])dnl
pushdef([idn], [yuck_slot_identifier(opt, cmd)])dnl
dnl
ifelse(cmd, [], [idn], [cmd.idn])[]dnl
dnl
popdef([idn])dnl
popdef([cmd])dnl
popdef([opt])dnl
])

## yuck_iftype([opt], [cmd], [type], [body], [[type], [body]]...)
define([yuck_iftype], [dnl
pushdef([opt], [$1])dnl
pushdef([cmd], [$2])dnl
pushdef([type], defn([YUCK.]cmd[.]opt[.type]))dnl
popdef([cmd])dnl
popdef([opt])dnl
[]ifelse(_splice(type, shift(shift($@))))[]dnl
popdef([type])dnl
])
define([_splice], [ifelse([$#], [3], [[$1], [$2], [$3]], [[$1], [$2], [$3], _splice([$1], shift(shift(shift($@))))])])

## yuck_cmds()
define([yuck_cmds], [defn([YUCK_CMD])])

## yuck_cmd
define([yuck_cmd], [YUCK_UMC[_]ifelse([$1], [], [NONE], [$1])])

## yuck_cmd_string
define([yuck_cmd_string], [defn([YUCK_STR.]$1)])

## yuck_shorts([cmd])
define([yuck_shorts], [defn([YUCK.]$1[.S])])

## yuck_longs([cmd])
define([yuck_longs], [defn([YUCK.]$1[.L])])

## yuck_idents([cmd])
define([yuck_idents], [defn([YUCK.]$1[.I])])


## coroutine stuff
define([yield], [goto $1; back_from_$1:])
define([coroutine], [define([this_coru], [$1])$1:])
define([resume], [goto back_from_[]this_coru])
define([resume_at], [goto $1])
define([quit], [goto out])

divert[]dnl
changequote
