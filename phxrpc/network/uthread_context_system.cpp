/*
Tencent is pleased to support the open source community by making 
PhxRPC available.
Copyright (C) 2016 THL A29 Limited, a Tencent company. 
All rights reserved.

Licensed under the BSD 3-Clause License (the "License"); you may 
not use this file except in compliance with the License. You may 
obtain a copy of the License at

https://opensource.org/licenses/BSD-3-Clause

Unless required by applicable law or agreed to in writing, software 
distributed under the License is distributed on an "AS IS" basis, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
implied. See the License for the specific language governing 
permissions and limitations under the License.

See the AUTHORS file for names of contributors.
*/

#include <unistd.h>
#include <ucontext.h>
#include <functional>
#include <assert.h>
#include "uthread_context_system.h"
#include "uthread_context_util.h"

namespace phxrpc
{

UThreadContextSystem::UThreadContextSystem(size_t stack_size, UThreadFunc_t func, void *args,
                                           UThreadDoneCallback_t callback, const bool need_stack_protect)
    : func_(func), args_(args), stack_(stack_size, need_stack_protect), callback_(callback)
{
  Make(func, args);
}

UThreadContextSystem::~UThreadContextSystem()
{
}

UThreadContext *UThreadContextSystem::DoCreate(size_t stack_size,
                                               UThreadFunc_t func, void *args, UThreadDoneCallback_t callback,
                                               const bool need_stack_protect)
{
  return new UThreadContextSystem(stack_size, func, args, callback, need_stack_protect);
}

void UThreadContextSystem::Make(UThreadFunc_t func, void *args)
{
  func_ = func;
  args_ = args;
  /// 将当前用户context保存到context_之中　
  getcontext(&context_);
  context_.uc_stack.ss_sp = stack_.top();
  context_.uc_stack.ss_size = stack_.size();
  context_.uc_stack.ss_flags = 0;
  // Here uc_link points to the context that will be resumed when the current context terminates
  /// uc_link指向当前context结束之后，将要resume的context
  context_.uc_link = GetMainContext();
  uintptr_t ptr = (uintptr_t) this;
  makecontext(&context_, (void (*)(void)) UThreadContextSystem::UThreadFuncWrapper,
              2, (uint32_t) ptr, (uint32_t) (ptr >> 32));
}

bool UThreadContextSystem::Resume()
{
  // int swapcontext(ucontext_t *oucp, ucontext_t *ucp)
  // The swapcontext() function saves the current context in the structure pointed to by oucp,
  // and then activates the context pointed to by ucp.
  ///  恢复context_指定的context, 继续运行, 将当前context保存到main_context之中
  swapcontext(GetMainContext(), &context_);
  return true;
}

bool UThreadContextSystem::Yield()
{
  /// 恢复main_context指定的context继续运行，将当前context保存到context_
  swapcontext(&context_, GetMainContext());
  return true;
}

ucontext_t *UThreadContextSystem::GetMainContext()
{
  /// 返回当前线程的main_context的地址
  static __thread ucontext_t main_context;
  return &main_context;
}

void UThreadContextSystem::UThreadFuncWrapper(uint32_t low32, uint32_t high32)
{
  uintptr_t ptr = (uintptr_t) low32 | ((uintptr_t) high32 << 32);
  // get class pointer
  UThreadContextSystem *uc = (UThreadContextSystem *) ptr;
  uc->func_(uc->args_);
  if (uc->callback_ != nullptr)
  {
    // complete, call callback function
    uc->callback_();
  }
}

} //namespace phxrpc
