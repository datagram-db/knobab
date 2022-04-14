explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, actions x
WHERE a.task = x.taska and EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.timex > a.timex)
GROUP BY x.taska, x.taskb;

explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, actions x
WHERE a.task = x.taska and EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.timex > a.timex)
GROUP BY x.taska, x.taskb;

explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, actions x
WHERE a.task = x.taska and EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.timex > a.timex)
GROUP BY x.taska, x.taskb;

explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, actions x
WHERE a.task = x.taska and EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.timex > a.timex)
GROUP BY x.taska, x.taskb;

explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, actions x
WHERE a.task = x.taska and EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.timex > a.timex)
GROUP BY x.taska, x.taskb;