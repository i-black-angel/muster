/**
 * Copyright 2018 reconova.com All Rights Reserved
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
#include "Thread.h"
#include <stdio.h>

namespace mpl {

void *Thread::threadRoutine(void *arg) {
    Thread *thread = (Thread *)arg;
    if (NULL != thread) thread->run();
    return NULL;
}

Thread::Thread() : _threadId(0), _isRunning(false) {
	pthread_attr_init(&_attr);
}

Thread::~Thread() {
	pthread_attr_destroy(&_attr);
}

int Thread::start() {
    if (_isRunning) return 0;

    _isRunning = true;
    int ret = pthread_create(&_threadId, NULL, threadRoutine, this);
    if (0 != ret) perror("pthread_create");
	return ret;
}

void Thread::stop() {
    if (!_isRunning) return;

    interrupt();
    stopHook();
    join();
}

void Thread::join()
{
    pthread_join(_threadId, NULL);
}

void Thread::detach()
{
    pthread_detach(_threadId);
}

Thread::DetachState Thread::detachState() const {
    int detachstate = PTHREAD_CREATE_JOINABLE;
    pthread_attr_getdetachstate(&_attr, &detachstate);
    if (PTHREAD_CREATE_DETACHED == detachstate) return kDetached;
    return kJoinable;
}

void Thread::setDetachState(Thread::DetachState val) {
    pthread_attr_setdetachstate(&_attr, static_cast<int>(val));
}

size_t Thread::guardSize() const {
    size_t guardsize = 0;
    pthread_attr_getguardsize(&_attr, &guardsize);
    return guardsize;
}

void Thread::setGuardSize(size_t val) {
    pthread_attr_setguardsize(&_attr, val);
}

size_t Thread::stackSize() const {
    size_t stacksize = 0;
    pthread_attr_getstacksize(&_attr, &stacksize);
    return stacksize;
}

void Thread::setStackSize(size_t val) {
    pthread_attr_setstacksize(&_attr, val);
}

int Thread::schedPriority() const {
    struct sched_param param;
    pthread_attr_getschedparam(&_attr, &param);
    return param.__sched_priority;
}

void Thread::setSchedPriority(int val) {
    struct sched_param param;
    param.__sched_priority = val;
    pthread_attr_setschedparam(&_attr, &param);
}

Thread::SchedulingAlgorithm Thread::schedPolicy() const {
    int policy = SCHED_OTHER;
    pthread_attr_getschedpolicy(&_attr, &policy);
    if (SCHED_FIFO == policy)
        return kSchedFIFO;
    else if (SCHED_RR == policy)
        return kSchedRR;
    else
        return kSchedOther;
}

void Thread::setSchedPolicy(Thread::SchedulingAlgorithm val) {
    pthread_attr_setschedpolicy(&_attr, static_cast<int>(val));
}

Thread::SchedulerInheritance Thread::inheritSched() const {
    int inherit = PTHREAD_INHERIT_SCHED;
    pthread_attr_getinheritsched(&_attr, &inherit);
    if (PTHREAD_EXPLICIT_SCHED == inherit) return kExplicitSched;
    return kInheritSched;
}

void Thread::setInheritSched(Thread::SchedulerInheritance val) {
    pthread_attr_setinheritsched(&_attr, static_cast<int>(val));
}

Thread::ScopeHandling Thread::scope() const {
    int scope = PTHREAD_SCOPE_SYSTEM;
    pthread_attr_getscope(&_attr, &scope);
    if (PTHREAD_SCOPE_PROCESS == scope) return kScopeProcess;
    return kScopeSystem;
}

void Thread::setScope(Thread::ScopeHandling val) {
    pthread_attr_setscope(&_attr, static_cast<int>(val));
}

}  // namespace mpl
