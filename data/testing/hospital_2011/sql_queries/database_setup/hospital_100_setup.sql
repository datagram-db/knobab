CREATE TABLE public.actions (
    taska character varying,
    taskb character varying 
);

\copy actions from '../../data/100/actions.tab' with delimiter E'\t' null as '';

CREATE TABLE public.log1 (
    event_id bigint NOT NULL,
    trace_id bigint,
    task character varying,
    timex bigint
);

\copy log1 from '../../data/100/log.tab' with delimiter E'\t' null as '';