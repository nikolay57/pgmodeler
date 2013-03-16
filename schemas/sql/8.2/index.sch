# SQL definition for indexes
# PostgreSQL Version: 8.2
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE ] %if @{unique} %then [UNIQUE ] %end [INDEX ]

%if @{concurrent} %then
 [ CONCURRENTLY ]
%end

@{name} [ ON ] @{table} $br

%if @{index-type} %then
 $tb [USING ] @{index-type} $br
%end

$tb ( @{elements} ) $br

%if @{expression} %then
  $tb (@{expression}) $sp 
%end

%if @{stg-params} %then
 %if@{factor} %then
   $tb [WITH (FILLFACTOR = ] @{factor} [)] $br
 %end
%end

%if @{tablespace} %then
 $tb [TABLESPACE ] @{tablespace} $br
%end

%if @{condition} %then
 $tb [WHERE (] @{condition} [)] $br
%end
;$br

%if @{comment} %then  @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br