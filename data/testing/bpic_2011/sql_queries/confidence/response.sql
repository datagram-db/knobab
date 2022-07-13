SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support,
((CAST(COUNT(*) AS FLOAT) /CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 WHERE task LIKE taska
GROUP BY trace_id) t2) AS FLOAT) /CAST( (SELECT COUNT(*) FROM (SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) AS Confidence
FROM log1 a, (SELECT a.task AS taska , b.task AS taskb FROM log1 a, log1 b  GROUP BY a.task , b.task) x
WHERE a.task = x.taska and EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.time > a.time)
GROUP BY x.taska, x.taskb