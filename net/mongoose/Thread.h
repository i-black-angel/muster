/**
 * Copyright 2020 Shusheng Shao <iblackangel@163.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>

namespace mpl {
class Thread {
public:
    enum DetachState {
        kJoinable,
        kDetached
    };

    enum SchedulingAlgorithm {
        kSchedOther = 0,
        kSchedFIFO  = 1,
        kSchedRR    = 2
    };

    enum SchedulerInheritance {
        kInheritSched,
        kExplicitSched
    };

    enum ScopeHandling {
        kScopeSystem,
        kScopeProcess
    };

    Thread();
    virtual ~Thread();
    virtual int init(void* priData) = 0;
    virtual void uninit() = 0;

    int start();
    void stop();
    pthread_t id() const { return _threadId; }

    void join();    // joinable
    void detach();  // detached

    // 线程分离状态
    DetachState detachState() const;
    void setDetachState(DetachState val);

    // 栈保护区大小，为线程提供至少 guardsize 字节的溢出保护区
    size_t guardSize() const;
    void setGuardSize(size_t val);

    // 线程堆栈大小
    size_t stackSize() const;
    void setStackSize(size_t val);

    // 线程调度优先级
    int schedPriority() const;
    void setSchedPriority(int val);

    // 线程调度策略
    SchedulingAlgorithm schedPolicy() const;
    void setSchedPolicy(SchedulingAlgorithm val);

    // 继承调度属性
    SchedulerInheritance inheritSched() const;
    void setInheritSched(SchedulerInheritance val);

    // 线程调度竞争范围
    ScopeHandling scope() const;
    void setScope(ScopeHandling val);

    bool isRunning() const { return _isRunning; }
    void interrupt() { _isRunning = false; }
protected:
    /**
     * example:
     * 
     *	void run() {
     *		while (isRunning()) {
     *			do_something();
     *		}
     *	}
     *
     */
    virtual void run() = 0;

    /**
     * 主要用于在 interrupt() 与 join() 之间
     * 如果继承类需要停止本类中特定的操作时使用， 比如唤醒信号量
     */
    virtual void stopHook() {}

private:
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    static void* threadRoutine(void* arg);

    pthread_t _threadId;    // thread id
    pthread_attr_t _attr;   // thread attribute
    bool _isRunning;
};
}  // namespace mpl

#endif /* _THREAD_H_ */
