changequote`'changequote([,])dnl
changecom([#])dnl
define([calloc], [malloc($1 * $2)])
define([realloc], [realloc($@)])
define([free], [free($@)])
