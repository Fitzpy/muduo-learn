// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.
/*计时器类*/
#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include <boost/noncopyable.hpp>

#include <muduo/base/Atomic.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>

namespace muduo
{
namespace net
{
///
/// Internal class for timer event.
///
class Timer : boost::noncopyable
{
 public:
  Timer(const TimerCallback& cb, Timestamp when, double interval)
    : callback_(cb),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),//如果间隔大于0，就重复
      sequence_(s_numCreated_.incrementAndGet())
  { }

  void run() const
  {
    callback_();
  }

  Timestamp expiration() const  { return expiration_; }//返回超时时刻
  bool repeat() const { return repeat_; }//返回是否重复
  int64_t sequence() const { return sequence_; }//返回定时器序号

  void restart(Timestamp now);//重新开始

  static int64_t numCreated() { return s_numCreated_.get(); }//返回最新的序号值

 private:
  const TimerCallback callback_;		// 定时器回调函数
  Timestamp expiration_;				// 下一次的超时时间戳类
  const double interval_;				// 超时时间间隔，如果是一次性定时器，该值为0
  const bool repeat_;					// 是否重复
  const int64_t sequence_;				// 定时器序号，不会重复

  static AtomicInt64 s_numCreated_;		// 定时器计数，当前已经创建的定时器数量
};
}
}
#endif  // MUDUO_NET_TIMER_H
