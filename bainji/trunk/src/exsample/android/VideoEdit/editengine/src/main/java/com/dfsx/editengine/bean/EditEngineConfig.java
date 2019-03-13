package com.dfsx.editengine.bean;

public class EditEngineConfig {
    private String cacheDir;
    private String logDir;

    EditEngineConfig() {

    }

    public String getCacheDir() {
        return cacheDir;
    }

    public String getLogDir() {
        return logDir;
    }

    public static class Builder {
        private EditEngineConfig config;

        public Builder() {
            config = new EditEngineConfig();
        }

        public Builder setCacheDir(String cacheDir) {
            config.cacheDir = cacheDir;
            return this;
        }

        public Builder setLogDir(String logDir) {
            config.logDir = logDir;
            return this;
        }

        public EditEngineConfig build() {
            return config;
        }
    }
}
