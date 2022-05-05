CREATE TABLE public.actions (
    taska character varying,
    taskb character varying 
);


\copy actions from '/home/samuelappleby/Documents/CodeBases/knobab/data/testing/hospital/10/actions.tab' with delimiter E'\t' null as '';

CREATE TABLE public.log1 (
    event_id bigint NOT NULL,
    trace_id bigint,
    task character varying,
    timex bigint
); 

\copy log1 from '/home/samuelappleby/Documents/CodeBases/knobab/data/testing/hospital/10/log.tab' with delimiter E'\t' null as '';
