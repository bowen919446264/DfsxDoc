package com.dfsx.editengine.test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class TestJava {

    public static void main(String[] args) {
        List<TestSort> list = new ArrayList<>();
        list.add(new TestSort(11, "1"));
        list.add(new TestSort(null, "2"));
        list.add(new TestSort(5, "3"));
        list.add(new TestSort(null, "4"));
        list.add(new TestSort(20, "5"));

        Collections.sort(list, new Comparator<TestSort>() {
            @Override
            public int compare(TestSort o1, TestSort o2) {
                Integer o1Count = o1.getCount();
                Integer o2Count = o2.getCount();
                if (o1Count == null || o2Count == null) {
                    if (o1Count == null && o2Count == null) {
                        return 0;
                    }
                    if (o1Count == null && o2Count != null) {
                        o1Count = o2Count + 1;
                    }
                    if (o2Count == null && o1Count != null) {
                        o2Count = o1Count + 1;
                    }

                }

                if (o1Count < o2Count) {
                    return -1;
                } else if (o1Count > o2Count) {
                    return 1;
                }
                return 0;
            }
        });

        for (
                TestSort sort : list)

        {
            System.out.println(sort.getName() + " ---- " + sort.getCount());
        }
    }


    static class TestSort {

        private Integer count;
        private String name;

        public TestSort(Integer count, String name) {
            this.count = count;
            this.name = name;
        }

        public Integer getCount() {
            return count;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }
    }
}
