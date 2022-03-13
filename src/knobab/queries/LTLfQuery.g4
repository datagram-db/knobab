grammar LTLfQuery;



query : 'INIT' declare_arguments              #init
      | 'END'  declare_arguments              #end
      | 'EXISTS' declare_arguments INTNUMBER  #exists
      | 'ABSENCE' declare_arguments INTNUMBER #absence
      | 'NEXT' query                          #next
      | query 'OR' THETA? query               #or
      | query 'AND' THETA? query              #and
      | query '->' THETA? query               #ifte
      | query 'U' THETA?  query               #ifte
      | 'G' query                             #box
      | 'F' query                             #diamond
      | '!u' query PRESERVE?                  #ifte
      | '(' query ')'                         #paren
      ;

declare_arguments : LEFT | RIGHT;

LPAREN : '(';
PRESERVE: 'PRESERVE';
THETA: 'THETA';
LEFT : 'L';
RIGHT: 'R';
INTNUMBER : ('0'..'9')+ ;
STRING : '"' (~[\\"] | '\\' [\\"])* '"';
SPACE : [ \t\n\r]+ -> skip;

COMMENT
    : '/*' .*? '*/' -> skip
;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
;
