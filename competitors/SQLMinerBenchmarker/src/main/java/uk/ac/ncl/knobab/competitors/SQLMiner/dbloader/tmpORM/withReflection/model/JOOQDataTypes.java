package uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model;

import org.jooq.DataType;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms.rdbms.DBMSInterface;

import java.lang.reflect.Field;

public class JOOQDataTypes {

    public static DataType<?> nameToDataType(DBMSInterface iface, String name) {
        Field[] fls = iface.getAssociatedTypes().getFields();
        DataType search = null;
        for (int i = 0; i<fls.length; i++) {
            Field f = fls[i];
            f.setAccessible(true);
            try {
                if ((search = ((DataType)f.get(null))).getTypeName().equals(name)) {
                    return search;
                }
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

}
