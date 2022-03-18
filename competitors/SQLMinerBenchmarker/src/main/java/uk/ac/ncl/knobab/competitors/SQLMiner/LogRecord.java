package uk.ac.ncl.knobab.competitors.SQLMiner;

import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.SQLType;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.Table;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmptables.SourceTabLoader;

import java.io.File;

@Table(sqlTable = "log1")
public class LogRecord extends SourceTabLoader {

    /**
     * The EventId in the SQLMiner context is a unique identifier for each record
     */
    @SQLType(type = "bigserial")
    public Long event_id;

    /**
     * Trace identification within the log
     */
    @SQLType(type = "bigint")
    public Long trace_id;

    /**
     * The act associated to the id
     */
    @SQLType(type = "varchar")
    public String task;

    /**
     * Trace identification within the log
     */
    @SQLType(type = "bigint")
    public Long time;

    public LogRecord() {
        event_id = 0L;
        trace_id = 0L;
        time = 0L;
        task = "";
    }

    public Long getIdentity() {
        return event_id;
    }

    @Override
    public void load(String[] args) {
        this.event_id = Long.valueOf(args[0]);
        this.trace_id = Long.valueOf(args[1]);
        this.time = Long.valueOf(args[3]);
        this.task = args[2];
    }

    @Override
    public File getFile(File parentFolder) {
        return new File(parentFolder, "log.tab");
    }

    @Override
    public LogRecord generateNew() {
        return new LogRecord();
    }
}
