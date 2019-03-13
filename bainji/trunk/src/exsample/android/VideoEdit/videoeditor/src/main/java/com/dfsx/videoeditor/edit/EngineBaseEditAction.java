package com.dfsx.videoeditor.edit;

import android.content.Context;

import com.dfsx.editengine.EngineProjectManager;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;

public abstract class EngineBaseEditAction extends BaseEditAction {
    protected EngineProjectManager engineProjectManager;

    public EngineBaseEditAction(Context context, ITimeLineUI timeLineUI, EngineProjectManager projectManager) {
        super(context, timeLineUI);
        this.engineProjectManager = projectManager;
    }
}
