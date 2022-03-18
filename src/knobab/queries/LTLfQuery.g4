grammar LTLfQuery;

query_plans : (query_plan)+
           ;

query_plan : AUTO_TIMED? STRING LPAREN declare_syntax+ RPAREN;

declare_syntax : 'template' STRING has_args? ':=' query;

has_args : 'args' INTNUMBER;

query : INIT TIMED?                      declare_arguments? declare_act_target?        #init
      | END  TIMED?                      declare_arguments? declare_act_target?        #end
      | LAST                                                declare_act_target?        #last
      | FIRST                                               declare_act_target?        #first
      | EXISTS NEGATED? INTNUMBER TIMED? declare_arguments? declare_act_target?        #exists
      | ABSENCE INTNUMBER TIMED?         declare_arguments? declare_act_target?        #absence
      | NEXT query                                                                     #next
      |<assoc=right> query OR TIMED? THETA? query                                      #or
      |<assoc=right> query AND TIMED? THETA? query                                     #and
      |<assoc=right> query '=>' TIMED? THETA? query                                    #implication
      |<assoc=right> IF TIMED? query THEN query THETA? ELSE query                      #ifte
      |<assoc=right> query UNTIL TIMED? THETA?  query                                  #until
      | BOX TIMED?  query                                                              #box
      | DIAMOND TIMED?   query                                                         #diamond
      | NEGATED TIMED? query PRESERVE?                                                 #not
      | '(' query ')'                                                                  #paren
      |<assoc=right> query '&Ft' THETA? query                                          #and_future
      |<assoc=right> query '&XGt' THETA? query                                         #and_next_globally
      |<assoc=right> query '&Gt' THETA? query                                          #and_globally
      ;


declare_arguments : '#' INTNUMBER;
declare_act_target : ACTIVATION | TARGET;

ACTIVATION: 'activation';
TARGET: 'target';
INIT: 'INIT';
END: 'END';
EXISTS: 'EXISTS';
ABSENCE: 'ABSENCE';
NEXT : 'NEXT';
OR : 'OR';
AND: 'AND';
FIRST: 'FIRST';
LAST: 'LAST';
IF : 'IF';
THEN: 'THEN';
ELSE: 'ELSE';
UNTIL: 'U';
BOX: 'G';
DIAMOND : 'F';
AUTO_TIMED: 'auto-timed';
LPAREN : '{';
RPAREN : '}';
PRESERVE: 'PRESERVE';
TIMED: 't';
THETA: 'THETA';
LEFT : 'L';
RIGHT: 'R';
MIDDLE: 'M';
NEGATED: '~';
INTNUMBER : ('0'..'9')+ ;
STRING : '"' (~[\\"] | '\\' [\\"])* '"';
SPACE : [ \t\n\r]+ -> skip;

COMMENT
    : '/*' .*? '*/' -> skip
;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
;
