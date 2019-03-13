package dfsx.com.videodemo.frag;

/**
 * RXBus的通用消息
 *
 * @param <T>
 */
public class RXBusMessage<T> {

    public long id;

    public String action;

    private T data;

    private Object tag;

    public RXBusMessage(T data) {
        this.data = data;
    }

    public RXBusMessage(long id, T data) {
        this.data = data;
        this.id = id;
    }

    public RXBusMessage(String action, T data) {
        this.data = data;
        this.action = action;
    }

    public T getData() {
        return data;
    }

    public Object getTag() {
        return tag;
    }

    public void setTag(Object tag) {
        this.tag = tag;
    }
}
