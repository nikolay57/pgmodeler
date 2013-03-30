# SQL definition for type roles
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

%if @{sql-disabled} %then
[/* The SQL code for this object was disabled on the editing form.] $br
%end

[CREATE ROLE ] @{name} [ WITH ] $br

# CREATEUSER | NOCREATEUSER --> Same as SUPERUSER | NOSUPERUSER
$tb %if @{superuser} %then SUPERUSER %else NOSUPERUSER %end $br
$tb %if @{createdb} %then CREATEDB %else NOCREATEDB %end $br
$tb %if @{createrole} %then CREATEROLE %else NOCREATEROLE %end $br
$tb %if @{inherit} %then INHERIT %else NOINHERIT %end $br
$tb %if @{login} %then LOGIN %else NOLOGIN %end $br
$tb %if @{connlimit} %then [CONNECTION LIMIT ] @{connlimit} %end $br

%if @{password} %then
 $tb
 %if @{encrypted} %then
   ENCRYPTED
 %else
   UNENCRYPTED
 %end
 [ PASSWORD ] '@{password}'
%end

%if @{validity} %then $br $tb [VALID UNTIL ] '@{validity}' %end

# IN GROUP rolename [, ...] --> Mesmo que IN ROLE
%if @{ref-roles} %then $br $tb [IN ROLE ] @{ref-roles} %end

# USER rolename [, ...] --> Mesmo que ROLE
%if @{member-roles} %then $br $tb [ROLE ] @{member-roles} %end
%if @{admin-roles} %then $br $tb [ADMIN ] @{admin-roles} %end
%if @{sysid} %then $br $tb [SYSID ] @{sysid} %end
; $br

%if @{comment} %then @{comment} %end

%if @{sql-disabled} %then
[ */] $br
%end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
