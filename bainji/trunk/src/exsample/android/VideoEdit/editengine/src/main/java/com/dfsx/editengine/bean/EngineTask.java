package com.dfsx.editengine.bean;

public class EngineTask {
    private String name;
    private Runnable task;

    public EngineTask(String name, Runnable task) {
        this.name = name;
        this.task = task;
    }

    public String getName() {
        return name;
    }

    public Runnable getTask() {
        return task;
    }
}
