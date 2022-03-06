explain analyse
SELECT taska , taskb ,
         (CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log WHERE
                 task LIKE taska ) AS FLOAT) ) AS Support
FROM log a, (SELECT a.task AS taska , b.task AS taskb FROM log a, log b  GROUP BY a.task , b.task ) x
WHERE a.task = x.taska and EXISTS (SELECT*FROM log b WHERE b .
task = x.taskb AND b.trace_id = a.trace_id AND b.time > a.time )
GROUP BY x.taska , x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log WHERE task LIKE taska ) AS FLOAT) ) = 1