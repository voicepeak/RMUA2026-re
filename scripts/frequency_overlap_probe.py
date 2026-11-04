"""Read-only GDB breakpoint on record_frequency_data; output active call overlap.

All-stop GDB changes scheduling and negative observations are inconclusive.
Tracks entry-to-return per thread; no target memory writes, save breakpoint traps.
"""
import gdb
import json
import time

active = {}
counts = {}

def now():
    return {'wall_unix_s': time.time(), 'monotonic_s': time.monotonic()}

def emit(event, **fields):
    print('FREQ_PROBE ' + json.dumps(dict(event=event, **now(), **fields), sort_keys=True), flush=True)

def tid():
    t = gdb.selected_thread()
    return t.ptid[1] if t is not None else -1

def topic_at_entry():
    try:
        address = int(gdb.parse_and_eval('$rdi'))
        mem = gdb.selected_inferior().read_memory(address, 16).tobytes()
        ptr = int.from_bytes(mem[:8], 'little')
        size = int.from_bytes(mem[8:16], 'little')
        if size > 512:
            return '<invalid-length:%d>' % size
        return gdb.selected_inferior().read_memory(ptr, size).tobytes().decode('utf-8','backslashreplace')
    except Exception as exc:
        return '<unreadable:%s>' % str(exc)

class Ret(gdb.FinishBreakpoint):
    def __init__(self, thread, topic, frame):
        super(Ret, self).__init__(frame, internal=True)
        self.silent = True
        self.thread_id = thread
        self.topic = topic
    def stop(self):
        entries = active.get(self.thread_id, [])
        if entries:
            entries.pop()
            if not entries: active.pop(self.thread_id, None)
        return False
    def out_of_scope(self):
        entries = active.get(self.thread_id, [])
        if entries:
            entries.pop()
            if not entries: active.pop(self.thread_id, None)
        emit('finish_out_of_scope', tid=self.thread_id, topic=self.topic)

class Entry(gdb.Breakpoint):
    def stop(self):
        thread = tid()
        topic = topic_at_entry()
        counts[topic] = counts.get(topic, 0) + 1
        others = [{'tid':t, 'topics':list(topics)} for t,topics in active.items() if t != thread]
        if others:
            emit('overlap', tid=thread, topic=topic, others=others, same_topic=any(topic in x['topics'] for x in others))
        if counts[topic] in (1, 1000, 10000):
            emit('count', tid=thread, topic=topic, count=counts[topic])
        try:
            Ret(thread, topic, gdb.newest_frame())
            active.setdefault(thread, []).append(topic)
        except Exception as exc:
            emit('finish_error', tid=thread, error=str(exc))
        return False

Entry('_ZN10airsim_ros21record_frequency_dataERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE', internal=False)
emit('ready')
