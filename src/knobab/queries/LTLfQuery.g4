grammar LTLfQuery;

query_plan : ('extensions' LPAREN declare_operator+ RPAREN)?
             ('declare' LPAREN declare_syntax+ RPAREN)?
           ;

declare_syntax : 'clause' LABEL '#args' INTNUMBER ('with' 'counter')? ':=' query;

query : 'INIT' declare_arguments                                            #init
      | 'END'  declare_arguments                                            #end
      | 'EXISTS' declare_arguments INTNUMBER                                #exists
      | 'ABSENCE' declare_arguments INTNUMBER                               #absence
      | 'NEXT' query                                                        #next
      | query 'OR' THETA? query                                             #or
      | query 'AND' THETA? query                                            #and
      | query '=>' THETA? query                                             #ifte
      | query 'U' THETA?  query                                             #ifte
      | 'G' query                                                           #box
      | 'F' query                                                           #diamond
      | '!' query PRESERVE?                                                 #ifte
      | 'expand' STRING ((query ',')* query) THETA? INTNUMBER? PRESERVE?    #callExpansion
      | '(' query ')'                                                       #paren
      ;

declare_operator : 'let ' STRING (declare_arguments | declare_arguments+ THETA?) INTNUMBER? PRESERVE? ':=' query ;

declare_arguments : LEFT | RIGHT;

LABEL: ('A'..'Z')[a-zA-Z]*;
LPAREN : '{';
RPAREN : '}';
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
