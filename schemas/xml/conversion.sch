# XML definition for type encoding conversion
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<conversion name=] "@{name}"
 [ src-encoding=] "@{src-encoding}"
 [ dst-encoding=] "@{dst-encoding}"

 %if @{protected} %then 
  [ protected=] "true"
 %end

  %if @{sql-disabled} %then
   [ sql-disabled=] "true"
  %end

 > $br

 @{schema}
 %if @{owner} %then @{owner} %end
 %if @{comment} %then @{comment} %end

 @{function}

</conversion>  $br $br
