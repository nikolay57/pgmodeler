# SQL definition for owner assignment on objects
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[ALTER ] @{sql-object} $sp

%if @{dif-sql} %then
 %if @{aggregate} %then @{name} [ (] @{types} [)] %end
 %if @{operator} %then @{signature} %end
 %if @{opclass} %then @{name} [ USING ] @{index-type} %end
 %if @{opfamily} %then @{name} [ USING ] @{index-type} %end
 %if @{function} %then @{signature} %end
%else
 @{name}
%end
[ OWNER TO ] @{owner}; $br
