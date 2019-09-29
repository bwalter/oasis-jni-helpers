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
#include "JniHelpersTestFixture.h"

#include "jnimock/jnimock.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "JniContext.h"
#include "JStringLocalRef.h"

#include <iostream>

using testing::_;
using testing::Return;
using namespace jnimock;

class JStringLocalRefTest: public JniHelpersTestFixture {};

TEST_F(JStringLocalRefTest, NullStringRefs) {

  EXPECT_CALL(*env, NewLocalRef(_))
    .Times(0);
  EXPECT_CALL(*env, DeleteLocalRef(_))
    .Times(0);

  JStringLocalRef stringRef1;
  EXPECT_EQ(nullptr, stringRef1.get());
  EXPECT_EQ(nullptr, stringRef1.m_sharedAutoRelease.get());

  JStringLocalRef stringRef2(stringRef1);
  EXPECT_EQ(nullptr, stringRef2.get());
  EXPECT_EQ(nullptr, stringRef1.m_sharedAutoRelease.get());
  EXPECT_EQ(nullptr, stringRef2.m_sharedAutoRelease.get());

  JStringLocalRef stringRef3(std::move(stringRef1));
  EXPECT_EQ(nullptr, stringRef3.get());
  EXPECT_EQ(nullptr, stringRef3.m_sharedAutoRelease.get());
}

TEST_F(JStringLocalRefTest, FromJavaString) {

  EXPECT_CALL(*env, NewLocalRef(_))
    .Times(0);
  EXPECT_CALL(*env, DeleteLocalRef(_))
    .Times(0);

  JStringLocalRef stringRef1(jniContext, jstring(543));
  EXPECT_EQ(jstring(543), stringRef1.get());
  EXPECT_NE(nullptr, stringRef1.m_sharedAutoRelease.get());
  EXPECT_EQ(1, stringRef1.m_sharedAutoRelease.use_count());

  EXPECT_CALL(*env, GetStringUTFChars(_, _))
    .Times(1)
    .WillOnce(Return("firstString"));

  // To UTF-8 string
  const char *utf8 = stringRef1.toUtf8Chars();

  EXPECT_EQ("firstString", std::string(utf8));

  EXPECT_CALL(*env, ReleaseStringUTFChars(_, _))
    .Times(1);
  EXPECT_CALL(*env, DeleteLocalRef(_))
    .Times(1);
}

