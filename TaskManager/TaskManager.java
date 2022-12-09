
/*

Task Manager

William Gregory (c) copyright 2075 - all rights reserved

first Java project.  for diversity!

*/

import java.util.*;

class Task {

    String name;
    int id;

    public void setup(String in_name, int in_id) {
        name = in_name;
        id = in_id;
        System.out.println("created: " + name);
    }

    public String return_name() {
        return name;
    }

    public int return_id() {
        return id;
    }

}

public class TaskManager {

    public static void main(String args[]) {

        // vector containing task
        Vector<Task> tasks = new Vector<Task>(0);

        // create a new task and add it to tasks
        Task t = new Task();
        t.setup("ME 311 - Study - Exam 1", 1);
        tasks.addElement(t);
        // again
        Task t2 = new Task();
        t2.setup("MOBIUS - Code - System", 2);
        tasks.addElement(t2);

        // list tasks size
        System.out.println("\ncurrent task count: " + tasks.size());

        // list tasks
        System.out.println("\nelements: ");
        for (int i=0; i<tasks.size(); ++i) {
            System.out.println(tasks.elementAt(i).return_name());
        }

    }
}