SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support,
((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM(SELECT trace_id FROM log1 WHERE task LIKE taska
GROUP BY trace_id) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) AS Confidence
FROM log1 a, actions x
WHERE a.task = x.taskb AND a.timex < ALL (SELECT timex FROM log1 b WHERE b.Task = x.taska AND b.trace_id = a.trace_id)
AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id) AND a.timex > ALL(SELECT timex FROM log1 b WHERE b.Task = x.taskb AND b.trace_id = a.trace_id)
GROUP BY x.taska, x.taskb