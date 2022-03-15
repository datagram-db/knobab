grammar LTLfQuery;

query_plan : (STRING LPAREN declare_syntax+ RPAREN)?
           ;

declare_syntax : 'template' LABEL ('args' INTNUMBER)? ('counter' INTNUMBER)? ':=' query;

query : 'INIT' declare_arguments                                            #init
      | 'END'  declare_arguments                                            #end
      | 'EXISTS' declare_arguments INTNUMBER                                #exists
      | 'ABSENCE' declare_arguments INTNUMBER                               #absence
      | 'NEXT' query                                                        #next
      | query 'OR' THETA? query                                             #or
      | query 'AND' THETA? query                                            #and
      | query '=>' THETA? query                                             #implication
      | 'IF' query 'then' query THETA? 'ELSE' query                         #ifte
      | query 'U' THETA?  query                                             #ifte
      | 'G' query                                                           #box
      | 'F' query                                                           #diamond
      | '!' query PRESERVE?                                                 #ifte
      | '(' query ')'                                                       #paren
      | query '&F' query THETA?                                             #and_future
      | query '&XG' query THETA?                                            #and_next_globally
      ;

declare_arguments : LEFT | MIDDLE | RIGHT;

LABEL: ('A'..'Z')[a-zA-Z]*;
LPAREN : '{';
RPAREN : '}';
PRESERVE: 'PRESERVE';
THETA: 'THETA';
LEFT : 'L';
RIGHT: 'R';
MIDDLE: 'M';
INTNUMBER : ('0'..'9')+ ;
STRING : '"' (~[\\"] | '\\' [\\"])* '"';
SPACE : [ \t\n\r]+ -> skip;

COMMENT
    : '/*' .*? '*/' -> skip
;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
;
