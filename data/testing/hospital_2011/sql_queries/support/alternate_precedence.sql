explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) AS Support
FROM log1 a, actions x
WHERE a.task = x.taskb AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id AND b.timex < a.timex)
AND NOT EXISTS(SELECT * FROM log1 b, log1 c WHERE c.trace_id = a.trace_id AND c.task = x.taskb AND b.trace_id = a.trace_id AND b.task = x.taska AND c.timex < a.timex AND c.timex > b.timex)
GROUP BY x.taska , x.taskb;