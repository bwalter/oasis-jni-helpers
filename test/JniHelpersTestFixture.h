/*
 * Copyright (C) 2019 Benoit Walter.
 *
 * Originally based on Duktape Android:
 * Copyright (C) 2015 Square, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "jnimock/jnimock.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "JniContext.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::SetArgPointee;
using namespace jnimock;

class JniHelpersTestFixture : public testing::Test {

protected:
  JniHelpersTestFixture() = default;
   
  void SetUp( ) {
    jvm = createJavaVMMock();
    env = createJNIEnvMock();

    EXPECT_CALL(*env, GetJavaVM(_))
      .Times(1)
      .WillOnce(DoAll(SetArgPointee<0>(jvm), Return(jint(0))));

    EXPECT_CALL(*env, GetVersion())
      .WillRepeatedly(Return(jint(69)));

    EXPECT_CALL(*jvm, GetEnv(_, jint(69)))
      .WillRepeatedly(DoAll(SetArgPointee<0>(env), Return(jint(0))));

    EXPECT_CALL(*jvm, AttachCurrentThread(_, _))
      .WillRepeatedly(DoAll(SetArgPointee<0>(env), Return(jint(0))));

    jniContext = new JniContext(env);
  }

  void TearDown() {
    destroyJNIEnvMock(env);
    destroyJavaVMMock(jvm);
  }

  JavaVMMock * jvm;
  JNIEnvMock * env;
  const JniContext * jniContext = nullptr;
};

