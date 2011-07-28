// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/stringprintf.h"
#include "base/values.h"
#include "chrome/common/pref_names.h"
#include "chrome/test/live_sync/live_sync_test.h"
#include "chrome/test/live_sync/preferences_helper.h"

class MultipleClientPreferencesSyncTest : public LiveSyncTest {
 public:
  MultipleClientPreferencesSyncTest() : LiveSyncTest(MULTIPLE_CLIENT) {}
  virtual ~MultipleClientPreferencesSyncTest() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(MultipleClientPreferencesSyncTest);
};

IN_PROC_BROWSER_TEST_F(MultipleClientPreferencesSyncTest, Sanity) {
  ASSERT_TRUE(SetupSync()) << "SetupSync() failed.";
  DisableVerifier();

  for (int i = 0; i < num_clients(); ++i) {
    ListValue urls;
    urls.Append(Value::CreateStringValue(
        base::StringPrintf("http://www.google.com/%d", i)));
    PreferencesHelper::ChangeListPref(i, prefs::kURLsToRestoreOnStartup, urls);
  }

  ASSERT_TRUE(AwaitQuiescence());
  ASSERT_TRUE(PreferencesHelper::ListPrefMatches(
      prefs::kURLsToRestoreOnStartup));
}
