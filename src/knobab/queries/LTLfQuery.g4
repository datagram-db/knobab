grammar LTLfQuery;

query_plans : (query_plan)+
           ;

query_plan : AUTO_TIMED? STRING LPAREN declare_syntax+ RPAREN;

declare_syntax : 'template' STRING has_args? ':=' query;

has_args : 'args' INTNUMBER;

query : 'INIT' declare_arguments                                            #init
      | 'END'  declare_arguments                                            #end
      | 'EXISTS' declare_arguments INTNUMBER                                #exists
      | 'ABSENCE' declare_arguments INTNUMBER                               #absence
      | 'NEXT' query                                                        #next
      |<assoc=right> query 'OR' TIMED? THETA? query                                      #or
      |<assoc=right> query 'AND' TIMED? THETA? query                                     #and
      |<assoc=right> query '=>' TIMED? THETA? query                                      #implication
      |<assoc=right> 'IF' TIMED? query 'THEN' query THETA? 'ELSE' query                  #ifte
      |<assoc=right> query  'U' TIMED? THETA?  query                                             #until
      | 'G' TIMED?  query                                                           #box
      | 'F' TIMED?   query                                                           #diamond
      | '!' TIMED? query PRESERVE?                                                 #not
      | '(' query ')'                                                       #paren
      |<assoc=right> query '&Ft' THETA? query                                             #and_future
      |<assoc=right> query '&XGt' THETA? query                                            #and_next_globally
      ;


declare_arguments : LEFT | MIDDLE | RIGHT;

AUTO_TIMED: 'auto-timed';
LABEL: ('A'..'Z')[a-zA-Z]*;
LPAREN : '{';
RPAREN : '}';
PRESERVE: 'PRESERVE';
TIMED: 't';
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
