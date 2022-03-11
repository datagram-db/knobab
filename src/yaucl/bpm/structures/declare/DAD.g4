/*
 * DAD.g
 * This file is part of bpm21
 *
 * Copyright (C) 2021 - Giacomo Bergami
 *
 * bpm21 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * bpm21 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bpm21. If not, see <http://www.gnu.org/licenses/>.
 */
grammar DAD;

data_aware_declare: (declare)*;

declare: name=LABEL '(' (fields ',')+ fields ')' ('where' prop)? #nary_prop
       | name=LABEL '(' fields ',' INTNUMBER ')' #unary_prop
       ;

fields: label=STRING ',' prop;

prop  : prop_within_dijunction '||' prop  #disj
      | prop_within_dijunction            #conj_or_atom
      | 'true'                            #top
      ;

prop_within_dijunction : atom                              #in_atom
                       | atom '&&' prop_within_dijunction  #atom_conj
                       ;

atom : (isnegated='~')? var rel (NUMBER | STRING | leftvar=var) ;

rel   : '<' #lt
      | '<=' #leq
      | '>' #gt
      | '>=' #geq
      | '=' #eq
      | '!=' #neq
      ;


var: 'var' STRING;
LABEL: ('A'..'Z')[a-zA-Z]*;
INTNUMBER : ('0'..'9')+ ;
NUMBER : '-'? INTNUMBER ('.' INTNUMBER)?;
STRING : '"' (~[\\"] | '\\' [\\"])* '"';
SPACE : [ \t\r\n]+ -> skip;

COMMENT
    : '/*' .*? '*/' -> skip
;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
;
