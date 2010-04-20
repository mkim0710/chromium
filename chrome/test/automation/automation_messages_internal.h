// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Defines the IPC messages used by the automation interface.

// This header is meant to be included in multiple passes, hence no traditional
// header guard.
// See ipc_message_macros.h for explanation of the macros and passes.

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "base/values.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/navigation_types.h"
#include "chrome/test/automation/autocomplete_edit_proxy.h"
#include "gfx/point.h"
#include "gfx/rect.h"
#include "googleurl/src/gurl.h"
#include "ipc/ipc_message_macros.h"

// NOTE: All IPC messages have either a routing_id of 0 (for asynchronous
//       messages), or one that's been assigned by the proxy (for calls
//       which expect a response).  The routing_id shouldn't be used for
//       any other purpose in these message types.

// NOTE: All the new IPC messages should go at the end (before IPC_END_MESSAGES)
//       The IPC message IDs are part of an enum and hence the value
//       assumed to be constant across the builds may change.
//       The messages AutomationMsg_WindowHWND* in particular should not change
//       since the PageCyclerReferenceTest depends on the correctness of the
//       message IDs across the builds.

IPC_BEGIN_MESSAGES(Automation)

  // This message is fired when the AutomationProvider is up and running
  // in the app (the app is not fully up at this point). The parameter to this
  // message is the version string of the automation provider. This parameter
  // is defined to be the version string as returned by
  // FileVersionInfo::file_version().
  // The client can choose to use this version string to decide whether or not
  // it can talk to the provider.
  IPC_MESSAGE_ROUTED1(AutomationMsg_Hello, std::string)

  // This message is fired when the initial tab(s) are finished loading.
  IPC_MESSAGE_ROUTED0(AutomationMsg_InitialLoadsComplete)

  // This message notifies the AutomationProvider to append a new tab the
  // window with the given handle. The return value contains the index of
  // the new tab, or -1 if the request failed.
  // The second parameter is the url to be loaded in the new tab.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_AppendTab, int, GURL, int)

  // This message requests the (zero-based) index for the currently
  // active tab in the window with the given handle. The return value contains
  // the index of the active tab, or -1 if the request failed.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_ActiveTabIndex, int, int)

  // This message notifies the AutomationProvider to active the tab.
  // The first parameter is the handle to window resource.
  // The second parameter is the (zero-based) index to be activated
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_ActivateTab, int, int, int)

  // This message requests the cookie value for given url in the
  // profile of the tab identified by the second parameter.  The first
  // parameter is the URL string. The response contains the length of the
  // cookie value string. On failure, this length = -1.
  IPC_SYNC_MESSAGE_ROUTED2_2(AutomationMsg_GetCookies, GURL, int,
                             int, std::string)

  // This message notifies the AutomationProvider to set and broadcast a cookie
  // with given name and value for the given url in the profile of the tab
  // identified by the third parameter. The first parameter is the URL
  // string, and the second parameter is the cookie name and value to be set.
  // The return value is a non-negative value on success.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_SetCookie, GURL, std::string,
                             int, int)

  // This message notifies the AutomationProvider to navigate to a specified
  // url in the tab with given handle. The first parameter is the handle to
  // the tab resource. The second parameter is the target url.  The return
  // value contains a status code which is nonnegative on success.
  // See AutomationMsg_NavigationResponseValues for the return value.
  //
  // Deprecated in favor of
  // AutomationMsg_NavigateToURLBlockUntilNavigationsComplete.
  // TODO(phajdan.jr): Remove when the reference build gets updated.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_NavigateToURL, int, GURL,
                             AutomationMsg_NavigationResponseValues)

  // This message is used to implement the asynchronous version of
  // NavigateToURL.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_NavigationAsync,
                             int /* tab handle */,
                             GURL,
                             bool /* result */)

  // This message notifies the AutomationProvider to navigate back in session
  // history in the tab with given handle. The first parameter is the handle
  // to the tab resource.
  // See AutomationMsg_NavigationResponseValues for the navigation response
  // values.
  //
  // Deprecated in favor of AutomationMsg_GoBackBlockUntilNavigationsComplete.
  // TODO(phajdan.jr): Remove when the reference build gets updated.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_GoBack, int,
                             AutomationMsg_NavigationResponseValues)

  // This message notifies the AutomationProvider to navigate forward in session
  // history in the tab with given handle. The first parameter is the handle
  // to the tab resource.
  // See AutomationMsg_NavigationResponseValues for the navigation response
  // values.
  //
  // Deprecated in favor of
  // AutomationMsg_GoForwardBlockUntilNavigationsComplete.
  // TODO(phajdan.jr): Remove when the reference build gets updated.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_GoForward, int,
                             AutomationMsg_NavigationResponseValues)

  // This message requests the number of browser windows that the app currently
  // has open.  The return value is the number of windows.
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_BrowserWindowCount, int)

  // This message requests the handle (int64 app-unique identifier) of the
  // window with the given (zero-based) index.  On error, the returned handle
  // value is 0.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_BrowserWindow, int, int)

  // This message requests the number of tabs in the window with the given
  // handle.  The return value contains the number of tabs, or -1 if the
  // request failed.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_TabCount, int, int)

  // This message requests the handle of the tab with the given (zero-based)
  // index in the given app window. First parameter specifies the given window
  // handle, second specifies the given tab_index. On error, the returned handle
  // value is 0.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_Tab, int, int, int)

  // This message requests the the title of the tab with the given handle.
  // The return value contains the size of the title string. On error, this
  // value should be -1 and empty string. Note that the title can be empty in
  // which case the size would be 0.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_TabTitle,
                             int,
                             int,
                             std::wstring)

  // This message requests the url of the tab with the given handle.
  // The return value contains a success flag and the URL string. The URL will
  // be empty on failure, and it still may be empty on success.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_TabURL,
                             int /* tab handle */,
                             bool /* success flag */,
                             GURL)

#if defined(OS_WIN)
  // TODO(estade): delete this unused message.
  IPC_SYNC_MESSAGE_ROUTED0_0(AutomationMsg_WindowHWND)

  // This message requests the HWND of the tab that corresponds
  // to the given automation handle.
  // The return value contains the HWND value, which is 0 if the call fails.
  //
  // TODO(estade): The only test that uses this message is
  // NPAPIVisiblePluginTester.SelfDeletePluginInvokeInSynchronousMouseMove. It
  // can probably be done in another way, and this can be removed.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_TabHWND,
                             int /* tab_handle */,
                             HWND /* win32 Window Handle */)
#endif  // defined(OS_WIN)

  // This message notifies the AutomationProxy that a handle that it has
  // previously been given is now invalid.  (For instance, if the handle
  // represented a window which has now been closed.)  The parameter
  // value is the handle.
  IPC_MESSAGE_ROUTED1(AutomationMsg_InvalidateHandle, int)

  // This message notifies the AutomationProvider that a handle is no
  // longer being used, so it can stop paying attention to the
  // associated resource.  The parameter value is the handle.
  IPC_MESSAGE_ROUTED1(AutomationMsg_HandleUnused, int)

  // This message tells the AutomationProvider to provide the given
  // authentication data to the specified tab, in response to an HTTP/FTP
  // authentication challenge.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_SetAuth,
                             int,  // tab handle
                             std::wstring,  // username
                             std::wstring,  // password
                             AutomationMsg_NavigationResponseValues)  // status

  // This message tells the AutomationProvider to cancel the login in the
  // specified tab.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_CancelAuth,
                             int,  // tab handle
                             AutomationMsg_NavigationResponseValues)  // status

  // Requests that the automation provider ask history for the most recent
  // chain of redirects coming from the given URL. The response must be
  // decoded by the caller manually; it contains an integer indicating the
  // number of URLs, followed by that many wstrings indicating a chain of
  // redirects. On failure, the count will be negative.
  IPC_SYNC_MESSAGE_ROUTED2_2(AutomationMsg_RedirectsFrom,
                             int,   // tab handle
                             GURL,  // source URL
                             bool /* succeeded */,
                             std::vector<GURL> /* redirects */)

  // This message asks the AutomationProvider whether a tab is waiting for
  // login info.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_NeedsAuth,
                             int,   // tab handle
                             bool)  // status

  // This message requests the AutomationProvider to apply a certain
  // accelerator. It is completely asynchronous with the resulting accelerator
  // action.
  IPC_SYNC_MESSAGE_ROUTED2_0(AutomationMsg_ApplyAccelerator,
                             int,  // window handle
                             int)  // accelerator id like (IDC_BACK,
                                   //  IDC_FORWARD, etc)
                                   // The list can be found at
                                   // chrome/app/chrome_dll_resource.h

  // This message requests that the AutomationProvider executes a JavaScript,
  // which is sent embedded in a 'javascript:' URL.
  // The javascript is executed in context of child frame whose xpath
  // is passed as parameter (context_frame). The execution results in
  // a serialized JSON string response.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_DomOperation,
                             int,           // tab handle
                             std::wstring,  // context_frame
                             std::wstring,  // the javascript to be executed
                             std::string)   // the serialized json string
                                            // containing the result of a
                                            // javascript execution

  // Is the Download Shelf visible for the specified browser?
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_ShelfVisibility,
                             int /* browser_handle */,
                             bool /* is_visible */)

  // This message requests the number of constrained windows in the tab with
  // the given handle.  The return value contains the number of constrained
  // windows, or -1 if the request failed.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_ConstrainedWindowCount,
                             int /* tab_handle */,
                             int /* constrained_window_count */)

  // This message requests the bounds of the specified View element in
  // window coordinates.
  // Request:
  //   int - the handle of the window in which the view appears
  //   int - the ID of the view, as specified in chrome/browser/view_ids.h
  //   bool - whether the bounds should be returned in the screen coordinates
  //          (if true) or in the browser coordinates (if false).
  // Response:
  //   bool - true if the view was found
  //   gfx::Rect - the bounds of the view, in window coordinates
  IPC_SYNC_MESSAGE_ROUTED3_2(AutomationMsg_WindowViewBounds, int, int,
                             bool, bool, gfx::Rect)

  // This message sets the bounds of the window.
  // Request:
  //   int - the handle of the window to resize
  //   gfx::Rect - the bounds of the window
  // Response:
  //   bool - true if the resize was successful
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_SetWindowBounds, int, gfx::Rect,
                             bool)

#if defined(OS_WIN)
  // TODO(port): Port these messages.
  //
  // This message requests that a drag be performed in window coordinate space
  // Request:
  //   int - the handle of the window that's the context for this drag
  //   std::vector<gfx::Point> - the path of the drag in window coordinate
  //                             space; it should have at least 2 points
  //                             (start and end)
  //   int - the flags which identify the mouse button(s) for the drag, as
  //         defined in chrome/views/event.h
  // Response:
  //   bool - true if the drag could be performed
  IPC_SYNC_MESSAGE_ROUTED4_1(AutomationMsg_WindowDrag,
                             int, std::vector<gfx::Point>, int, bool, bool)
#endif  // defined(OS_WIN)

  // Similar to AutomationMsg_InitialLoadsComplete, this indicates that the
  // new tab ui has completed the initial load of its data.
  // Time is how many milliseconds the load took.
  IPC_MESSAGE_ROUTED1(AutomationMsg_InitialNewTabUILoadComplete,
                      int /* time */)

  // This message starts a find within a tab corresponding to the supplied
  // tab handle. The return value contains the number of matches found on the
  // page within the tab specified. The parameter 'search_string' specifies
  // what string to search for, 'forward' specifies whether to search in
  // forward direction (1=forward, 0=back), 'match_case' specifies case
  // sensitivity
  // (1=case sensitive, 0=case insensitive). If an error occurs, matches_found
  // will be -1.
  //
  // NOTE: This message has been deprecated, please use the new message
  // AutomationMsg_Find below.
  //
  IPC_SYNC_MESSAGE_ROUTED4_2(AutomationMsg_FindInPage,   // DEPRECATED.
                             int, /* tab_handle */
                             std::wstring, /* find_request */
                             int, /* forward */
                             int /* match_case */,
                             int /* active_ordinal */,
                             int /* matches_found */)

  // This message sends a inspect element request for a given tab. The response
  // contains the number of resources loaded by the inspector controller.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_InspectElement,
                             int, /* tab_handle */
                             int, /* x */
                             int  /* y */,
                             int)

  // This message requests the process ID of the tab that corresponds
  // to the given automation handle.
  // The return value has an integer corresponding to the PID of the tab's
  // renderer, 0 if the tab currently has no renderer process, or -1 on error.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_TabProcessID,
                             int /* tab_handle */,
                             int /* process ID */)

  // This tells the browser to enable or disable the filtered network layer.
  IPC_MESSAGE_ROUTED1(AutomationMsg_SetFilteredInet,
                      bool /* enabled */)

  // Gets the directory that downloads will occur in for the active profile.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_DownloadDirectory,
                             int /* tab_handle */,
                             FilePath /* directory */)

  // This message requests the id of the view that has the focus in the
  // specified window. If no view is focused, -1 is returned.  Note that the
  // window should either be a ViewWindow or a Browser.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_GetFocusedViewID,
                             int /* view_handle */,
                             int /* focused_view_id */)

  // This message shows/hides the window.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_SetWindowVisible,
                             int /* view_handle */,
                             bool /* visible */,
                             bool /* success */)

  // Gets the active status of a window.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_IsWindowActive,
                             int /* view_handle */,
                             bool /* success */,
                             bool /* active */)

  // Makes the specified window the active window.
  IPC_SYNC_MESSAGE_ROUTED1_0(AutomationMsg_ActivateWindow,
                             int /* view_handle */)

  // Opens a new browser window.
  // TODO(sky): remove this and replace with OpenNewBrowserWindowOfType.
  // Doing this requires updating the reference build.
  IPC_SYNC_MESSAGE_ROUTED1_0(AutomationMsg_OpenNewBrowserWindow,
                             bool /* show */ )

  // This message requests the handle (int64 app-unique identifier) of the
  // current active top window.  On error, the returned handle value is 0.
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_ActiveWindow, int)

  // This message requests the browser associated with the specified window
  // handle.
  // The return value contains a success flag and the handle of the browser.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_BrowserForWindow,
                             int /* window handle */,
                             bool /* success flag */,
                             int /* browser handle */)

  // This message requests the window associated with the specified browser
  // handle.
  // The return value contains a success flag and the handle of the window.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_WindowForBrowser,
                             int /* browser handle */,
                             bool /* success flag */,
                             int /* window handle */)

  // This message requests the AutocompleteEdit associated with the specified
  // browser handle.
  // The return value contains a success flag and the handle of the omnibox.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_AutocompleteEditForBrowser,
                             int /* browser handle */,
                             bool /* success flag */,
                             int /* AutocompleteEdit handle */)

#if defined(OS_WIN)
  // TODO(estade): This message is defined later on for Mac and Linux. This is
  // to avoid adding a new IPC in the middle for those platforms (see comment
  // at top). The message is exactly the same, so they should be remerged when
  // all messages in this file have been made cross-platform (at which point we
  // will need to check in new reference builds).
  //
  // This message requests that a mouse click be performed in window coordinate
  // space.
  // Request:
  //   int - the handle of the window that's the context for this click
  //   gfx::Point - the point to click
  //   int - the flags which identify the mouse button(s) for the click, as
  //       defined in chrome/views/event.h
  IPC_MESSAGE_ROUTED3(AutomationMsg_WindowClick, int, gfx::Point, int)
#endif  // defined(OS_WIN)

  // This message requests that a key press be performed.
  // Request:
  //   int - the handle of the window that's the context for this click
  //   int - the base::KeyboardCode of the key that was pressed.
  //   int - the flags which identify the modifiers (shift, ctrl, alt)
  //         associated for, as defined in chrome/views/event.h
  IPC_MESSAGE_ROUTED3(AutomationMsg_WindowKeyPress, int, int, int)

  // This message notifies the AutomationProvider to create a tab which is
  // hosted by an external process.
  // Request:
  //   ExternalTabSettings - settings for external tab
  IPC_SYNC_MESSAGE_ROUTED1_3(AutomationMsg_CreateExternalTab,
                             IPC::ExternalTabSettings  /* settings*/,
                             gfx::NativeWindow  /* Tab container window */,
                             gfx::NativeWindow  /* Tab window */,
                             int  /* Handle to the new tab */)

  // This message notifies the AutomationProvider to navigate to a specified
  // url in the external tab with given handle. The first parameter is the
  // handle to the tab resource. The second parameter is the target url.
  // The third parameter is the referrer.
  // The return value contains a status code which is nonnegative on success.
  // see AutomationMsg_NavigationResponseValues for the navigation response.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_NavigateInExternalTab,
                             int,
                             GURL,
                             GURL,
                             AutomationMsg_NavigationResponseValues)

  // This message is an outgoing message from Chrome to an external host.
  // It is a notification that the NavigationState was changed
  // Request:
  //   -int: The flags specifying what changed
  //         (see TabContents::InvalidateTypes)
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED3(AutomationMsg_NavigationStateChanged,
                      int,  // tab handle
                      int,  // TabContents::InvalidateTypes
                      IPC::NavigationInfo)  // title, url etc.

  // This message is an outgoing message from Chrome to an external host.
  // It is a notification that the target URL has changed (the target URL
  // is the URL of the link that the user is hovering on)
  // Request:
  //   -int: The tab handle
  //   -std::wstring: The new target URL
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED2(AutomationMsg_UpdateTargetUrl, int, std::wstring)

  // This message notifies the AutomationProvider to show the specified html
  // text in an interstitial page in the tab with given handle. The first
  // parameter is the handle to the tab resource. The second parameter is the
  // html text to be displayed.
  // The return value contains a success flag.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_ShowInterstitialPage,
                             int,
                             std::string,
                             AutomationMsg_NavigationResponseValues)

  // This message notifies the AutomationProvider to hide the current
  // interstitial page in the tab with given handle. The parameter is the
  // handle to the tab resource.
  // The return value contains a success flag.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_HideInterstitialPage, int,
                             bool)

  // This message requests that a tab be closed.
  // Request:
  //   - int: handle of the tab to close
  //   - bool: if true the proxy blocks until the tab has completely closed,
  //           otherwise the proxy only blocks until it initiates the close.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_CloseTab, int, bool, bool)

  // This message requests that the browser be closed.
  // Request:
  //   - int: handle of the browser which contains the tab
  // Response:
  //  - bool: whether the operation was successfull.
  //  - bool: whether the browser process will be terminated as a result (if
  //          this was the last closed browser window).
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_CloseBrowser, int, bool,
                             bool)

  IPC_MESSAGE_ROUTED1(AutomationMsg_CloseBrowserRequestAsync, int)

  // Unused.
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED1(AutomationMsg_Unused, int)

#if defined(OS_WIN)
  // TODO(port): Port these messages.
  //
  // This message is an outgoing message from Chrome to an external host.
  // It is a request to process a keyboard accelerator.
  // Request:
  //   -int: Tab handle
  //   -MSG: The keyboard message
  // Response:
  //   None expected
  // TODO(sanjeevr): Ideally we need to add a response from the external
  // host saying whether it processed the accelerator
  IPC_MESSAGE_ROUTED2(AutomationMsg_HandleAccelerator, int, MSG)

  // This message is sent by the container of an externally hosted tab to
  // reflect any accelerator keys that it did not process. This gives the
  // tab a chance to handle the keys
  // Request:
  //   - int: handle of the tab
  //   -MSG: The keyboard message that the container did not handle
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED2(AutomationMsg_ProcessUnhandledAccelerator, int, MSG)
#endif  // defined(OS_WIN)

  // Sent by the external tab to the host to notify that the user has tabbed
  // out of the tab.
  // Request:
  //   - int: Tab handle
  //   - bool: |reverse| set to true when shift-tabbing out of the tab, false
  //    otherwise.
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED2(AutomationMsg_TabbedOut, int, bool)

  // Sent by the external tab host to ask focus to be set to either the first
  // or last element on the page.
  // Request:
  //   - int: handle of the tab
  //   - bool: |reverse|
  //      true: Focus will be set to the last focusable element
  //      false: Focus will be set to the first focusable element
  //   - bool: |restore_focus_to_view|
  //      true: The renderer view associated with the current tab will be
  //            infomed that it is receiving focus.
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED3(AutomationMsg_SetInitialFocus, int, bool, bool)

  // This message is an outgoing message from Chrome to an external host.
  // It is a request to open a url
  // Request:
  //   -int: Tab handle
  //   -GURL: The URL to open
  //   -GURL: The referrer
  //   -int: The WindowOpenDisposition that specifies where the URL should
  //         be opened (new tab, new window etc).
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED4(AutomationMsg_OpenURL, int, GURL, GURL, int)

  // This message requests the provider to wait until the specified tab has
  // finished restoring after session restore.
  // Request:
  //   - int: handle of the tab
  // Response:
  //  - bool: whether the operation was successful.
  IPC_SYNC_MESSAGE_ROUTED1_0(AutomationMsg_WaitForTabToBeRestored, int)

  // This message is an outgoing message from Chrome to an external host.
  // It is a notification that a navigation happened
  // Request:
  //   -int:  Tab handle
  //
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED2(AutomationMsg_DidNavigate, int, IPC::NavigationInfo)

  // This message requests the different security states of the page displayed
  // in the specified tab.
  // Request:
  //   - int: handle of the tab
  // Response:
  //  - bool: whether the operation was successful.
  //  - SecurityStyle: the security style of the tab.
  //  - int: the status of the server's ssl cert (0 means no errors or no ssl
  //         was used).
  //  - int: the mixed content state, 0 means no mixed/unsafe contents.

  IPC_SYNC_MESSAGE_ROUTED1_4(AutomationMsg_GetSecurityState,
                             int,
                             bool,
                             SecurityStyle,
                             int,
                             int)

  // This message requests the page type of the page displayed in the specified
  // tab (normal, error or interstitial).
  // Request:
  //   - int: handle of the tab
  // Response:
  //  - bool: whether the operation was successful.
  //  - NavigationEntry::PageType: the type of the page currently displayed.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_GetPageType, int, bool,
                             NavigationEntry::PageType)

  // This message simulates the user action on the SSL blocking page showing in
  // the specified tab.  This message is only effective if an interstitial page
  // is showing in the tab.
  // Request:
  //   - int: handle of the tab
  //   - bool: whether to proceed or abort the navigation
  // Response:
  //  - AutomationMsg_NavigationResponseValues: result of the operation.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_ActionOnSSLBlockingPage, int, bool,
                             AutomationMsg_NavigationResponseValues)

  // Message to request that a browser window is brought to the front and
  // activated.
  // Request:
  //   - int: handle of the browser window.
  // Response:
  //   - bool: True if the browser is brought to the front.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_BringBrowserToFront, int, bool)

  // Message to request whether a certain item is enabled of disabled in the
  // "Page" menu in the browser window
  //
  // Request:
  //   - int: handle of the browser window.
  //   - int: IDC message identifier to query if enabled
  // Response:
  //   - bool: True if the command is enabled on the Page menu
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_IsPageMenuCommandEnabled, int, int,
                             bool)

  // This message notifies the AutomationProvider to print the tab with given
  // handle. The first parameter is the handle to the tab resource.  The
  // return value contains a bool which is true on success.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_PrintNow, int, bool)

  // This message notifies the AutomationProvider to reload the current page in
  // the tab with given handle. The first parameter is the handle to the tab
  // resource.  The return value contains a status code which is nonnegative on
  // success.
  // see AutomationMsg_NavigationResponseValues for the navigation response.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_Reload, int,
                             AutomationMsg_NavigationResponseValues)

  // This message requests the handle (int64 app-unique identifier) of the
  // last active browser window, or the browser at index 0 if there is no last
  // active browser, or it no longer exists. Returns 0 if no browser windows
  // exist.
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_LastActiveBrowserWindow, int)

  // This message notifies the AutomationProvider to save the page with given
  // handle. The first parameter is the handle to the tab resource. The second
  // parameter is the main HTML file name. The third parameter is the directory
  // for saving resources. The fourth parameter is the saving type: 0 for HTML
  // only; 1 for complete web page.
  // The return value contains a bool which is true on success.
  IPC_SYNC_MESSAGE_ROUTED4_1(AutomationMsg_SavePage, int, FilePath, FilePath,
                             int, bool)

  // This message requests the text currently being displayed in the
  // AutocompleteEdit.  The parameter is the handle to the AutocompleteEdit.
  // The return value is a string indicating the text in the AutocompleteEdit.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_AutocompleteEditGetText,
                             int /* autocomplete edit handle */,
                             bool /* the requested autocomplete edit exists */,
                             std::wstring /* omnibox text */)

  // This message sets the text being displayed in the AutocompleteEdit.  The
  // first parameter is the handle to the omnibox and the second parameter is
  // the text to be displayed in the AutocompleteEdit.
  // The return value has no parameters and is returned when the operation has
  // completed.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_AutocompleteEditSetText,
                             int /* autocomplete edit handle */,
                             std::wstring /* text to set */,
                             bool /* the requested autocomplete edit exists */)

  // This message requests if a query to a autocomplete provider is still in
  // progress.  The first parameter in the request is the handle to the
  // autocomplete edit.
  // The first return value indicates if the request succeeded.
  // The second return value indicates if a query is still in progress.
  IPC_SYNC_MESSAGE_ROUTED1_2( \
      AutomationMsg_AutocompleteEditIsQueryInProgress,
      int /* autocomplete edit handle*/,
      bool /* the requested autocomplete edit exists */,
      bool /* indicates if a query is in progress */)

  // This message requests a list of the autocomplete messages currently being
  // displayed by the popup.  The parameter in the request is a handle to the
  // autocomplete edit.
  // The first return value indicates if the request was successful, while
  // while the second is the actual list of matches.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_AutocompleteEditGetMatches,
                             int /* autocomplete edit handle*/,
                             bool /* the requested autocomplete edit exists */,
                             std::vector<AutocompleteMatchData> /* matches */)

  // This message requests the execution of a browser command in the browser
  // for which the handle is specified.
  // The return value contains a boolean, whether the command was dispatched.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_WindowExecuteCommandAsync,
                             int /* automation handle */,
                             int /* browser command */,
                             bool /* success flag */)

  // This message requests the execution of a browser command in the browser
  // for which the handle is specified.
  // The return value contains a boolean, whether the command was dispatched
  // and successful executed.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_WindowExecuteCommand,
                             int /* automation handle */,
                             int /* browser command */,
                             bool /* success flag */)


  // This message opens the Find window within a tab corresponding to the
  // supplied tab handle.
  IPC_MESSAGE_ROUTED1(AutomationMsg_OpenFindInPage,
                      int /* tab_handle */)

  // Posts a message from external host to chrome renderer.
  IPC_MESSAGE_ROUTED4(AutomationMsg_HandleMessageFromExternalHost,
                      int /* automation handle */,
                      std::string /* message */,
                      std::string /* origin */,
                      std::string /* target */)

  // A message for an external host.
  IPC_MESSAGE_ROUTED4(AutomationMsg_ForwardMessageToExternalHost,
                      int, /* handle */
                      std::string /* message */,
                      std::string /* origin */,
                      std::string /* target */)

  // This message starts a find within a tab corresponding to the supplied
  // tab handle. The parameter |request| specifies what to search for.
  // If an error occurs, |matches_found| will be -1.
  //
  IPC_SYNC_MESSAGE_ROUTED2_2(AutomationMsg_Find,
                             int /* tab_handle */,
                             AutomationMsg_Find_Params /* params */,
                             int /* active_ordinal */,
                             int /* matches_found */)

  // Is the Find window fully visible (and not animating) for the specified
  // tab?
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_FindWindowVisibility,
                             int /* tab_handle */,
                             bool /* is_visible */)

  // Where is the Find window located. |x| and |y| will be -1, -1 on failure.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_FindWindowLocation,
                             int /* tab_handle */,
                             int /* x */,
                             int /* y */)

  // Is the Bookmark bar visible? The return value will indicate whether it is
  // visible or not and whether it is being animated into (or out of its place).
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_BookmarkBarVisibility,
                             int /* browser_handle */,
                             bool, /* is_visible */
                             bool  /* still_animating */)

  // This message requests the number of related info bars opened.  It
  // returns -1 if an error occurred.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_GetInfoBarCount,
                             int /* tab_handle */,
                             int /* info bar count */)

  // This message triggers the action associated with the "accept" button in
  // the info-bar at the specified index.  If |wait for navigation| is true, it
  // won't return until a navigation has occurred.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_ClickInfoBarAccept,
                             int /* tab_handle */,
                             int /* info bar index */,
                             bool /* wait for navigation */,

                             /* navigation result */
                             AutomationMsg_NavigationResponseValues)

  // This message retrieves the last time a navigation occurred in the specified
  // tab.  The value is intended to be used with WaitForNavigation.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_GetLastNavigationTime,
                             int /* tab_handle */,
                             int64 /* last navigation time */)

  // This messages is used to block until a new navigation occurs (if there is
  // none more recent then the time specified).
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_WaitForNavigation,
                             int /* tab_handle */,
                             int64 /* last navigation time */,

                             /* navigation result */
                             AutomationMsg_NavigationResponseValues)

  // This messages sets an int-value preference.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_SetIntPreference,
                             int /* browser handle */,
                             std::wstring /* pref name */,
                             int /* value */,
                             bool /* success */)

  // Queries whether an app modal dialog is currently being shown. (i.e. a
  // javascript alert) and which buttons it contains.
  IPC_SYNC_MESSAGE_ROUTED0_2(AutomationMsg_ShowingAppModalDialog,
                             bool /* showing dialog */,
                             int /* view::DelegateDialog::DialogButton */)

  // This message triggers the specified button for the currently showing
  // modal dialog.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_ClickAppModalDialogButton,
                             int /* view::DelegateDialog::DialogButton */,
                             bool /* success */)

  // This messages sets a string-value preference.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_SetStringPreference,
                             int /* browser handle */,
                             std::wstring /* pref name */,
                             std::wstring /* pref value */,
                             bool)

  // This messages gets a boolean-value preference.
  IPC_SYNC_MESSAGE_ROUTED2_2(AutomationMsg_GetBooleanPreference,
                             int /* browser handle */,
                             std::wstring /* pref name */,
                             bool /* success */,
                             bool /* pref value */)

  // This messages sets a boolean-value preference.
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_SetBooleanPreference,
                             int /* browser handle */,
                             std::wstring /* pref name */,
                             bool /* pref value */,
                             bool /* success */)

  // Queries the current used encoding name of the page in the specified
  // web content tab.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_GetPageCurrentEncoding,
                             int /* tab handle */,
                             std::string /* current used encoding name */)

  // Uses the specified encoding to override the encoding of the page in the
  // specified web content tab.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_OverrideEncoding,
                             int /* tab handle */,
                             std::string /* overrided encoding name */,
                             bool /* success */)

  // Used to disable the dialog box that prompts the user for a path when
  // saving a web page.
  IPC_SYNC_MESSAGE_ROUTED1_0(AutomationMsg_SavePackageShouldPromptUser,
                             bool /* false if we want to not show the dialog */)

  // This message is an outgoing message from Chrome to an external host.
  // It is a notification that a navigation failed
  // Request:
  //   -int : Tab handle
  //   -int : The status code.
  //   -GURL:  The URL we failed to navigate to.
  // Response:
  //   None expected
  IPC_MESSAGE_ROUTED3(AutomationMsg_NavigationFailed, int, int, GURL)

#if defined(OS_WIN)
  // This message is an outgoing message from an automation client to Chrome.
  // It is used to reposition a chrome tab window.
  IPC_MESSAGE_ROUTED2(AutomationMsg_TabReposition,
                      int /* tab handle */,
                      IPC::Reposition_Params /* SetWindowPos params */)
#endif  // defined(OS_WIN)

  // Gets the title of the top level browser window.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_WindowTitle,
                             int /* automation handle */,
                             string16 /* title text */ )

  // Tab load complete
  IPC_MESSAGE_ROUTED2(AutomationMsg_TabLoaded,
                      int,  // tab handle
                      GURL)

  // This message requests the tabstrip index of the tab with the given handle.
  // The return value contains the index, which will be -1 on failure.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_TabIndex, int, int)

  // This message requests the handle (int64 app-unique identifier) of
  // a valid normal browser window, i.e. normal type and non-incognito mode.
  // On error, the returned handle value is 0.
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_FindNormalBrowserWindow, int)

  // This message requests the number of normal browser windows, i.e. normal
  // type and non-incognito mode that the app currently has open.  The return
  // value is the number of windows.
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_NormalBrowserWindowCount, int)

  // Used to put the browser into "extension automation mode" for a given
  // set of Chrome Extensions API functions for the current profile, or turn
  // off automation mode.  The specified tab is used as the conduit for all
  // automated API functions.  It must be an external tab (as in
  // AutomationMsg_CreateExternalTab).
  IPC_MESSAGE_ROUTED2(AutomationMsg_SetEnableExtensionAutomation,
                      // Tab handle.
                      int,
                      // Empty to disable automation, non-empty to enable
                      // automation of the specified API functions, single
                      // entry of "*" to enable automation of all API
                      // functions.
                      std::vector<std::string>)

  // This message tells the browser to start using the new proxy configuration
  // represented by the given JSON string. The parameters used in the JSON
  // string are defined in automation_constants.h.
  IPC_MESSAGE_ROUTED1(AutomationMsg_SetProxyConfig,
                      std::string /* proxy_config_json_string */)

  // Sets Download Shelf visibility for the specified browser.
  IPC_SYNC_MESSAGE_ROUTED2_0(AutomationMsg_SetShelfVisibility,
                             int /* browser_handle */,
                             bool /* is_visible */)

  // This message requests the number of blocked popups in a certain tab with
  // the given handle. The return value is the number of blocked popups, or -1
  // if this request failed.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_BlockedPopupCount,
                             int /* tab_handle */,
                             int /* blocked_popup_count */)

  // This message retrieves the locale of the browser process.  On success
  // |chrome_locale| will contain the locale as reported by ICU.  On failure
  // |chrome_locale| is the empty string.
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_GetBrowserLocale,
                             string16 /* chrome_locale */)

#if defined(OS_WIN)
  IPC_MESSAGE_ROUTED4(AutomationMsg_ForwardContextMenuToExternalHost,
                      int /* tab_handle */,
                      HANDLE /* source menu handle */,
                      int    /* align flags */,
                      IPC::ContextMenuParams /* params */)

  IPC_MESSAGE_ROUTED2(AutomationMsg_ForwardContextMenuCommandToChrome,
                      int /* tab_handle */,
                      int /* selected_command */)
#endif  // OS_WIN

  // A URL request to be fetched via automation
  IPC_MESSAGE_ROUTED3(AutomationMsg_RequestStart,
                      int /* tab_handle */,
                      int /* request_id */,
                      IPC::AutomationURLRequest /* request */)

  // Read data from a URL request to be fetched via automation
  IPC_MESSAGE_ROUTED3(AutomationMsg_RequestRead,
                      int /* tab_handle */,
                      int /* request_id */,
                      int /* bytes_to_read */)

  // Response to a AutomationMsg_RequestStart message
  IPC_MESSAGE_ROUTED3(AutomationMsg_RequestStarted,
                      int /* tab_handle */,
                      int /* request_id */,
                      IPC::AutomationURLResponse /* response */)

  // Data read via automation
  IPC_MESSAGE_ROUTED3(AutomationMsg_RequestData,
                      int /* tab_handle */,
                      int /* request_id */,
                      std::string /* data */)

  IPC_MESSAGE_ROUTED3(AutomationMsg_RequestEnd,
                      int /* tab_handle */,
                      int /* request_id */,
                      URLRequestStatus /* status */)

  IPC_MESSAGE_ROUTED1(AutomationMsg_PrintAsync,
                      int /* tab_handle */)

  IPC_MESSAGE_ROUTED3(AutomationMsg_SetCookieAsync,
                      int /* tab_handle */,
                      GURL /* url */,
                      std::string /* cookie */)

  IPC_MESSAGE_ROUTED1(AutomationMsg_SelectAll,
                      int /* tab handle */)

  IPC_MESSAGE_ROUTED1(AutomationMsg_Cut,
                      int /* tab handle */)

  IPC_MESSAGE_ROUTED1(AutomationMsg_Copy,
                      int /* tab handle */)

  IPC_MESSAGE_ROUTED1(AutomationMsg_Paste,
                      int /* tab handle */)

  IPC_MESSAGE_ROUTED1(AutomationMsg_ReloadAsync,
                      int /* tab handle */)

  IPC_MESSAGE_ROUTED1(AutomationMsg_StopAsync,
                      int /* tab handle */)

  // Returns the number of times a filter was used to service an URL request.
  // See AutomationMsg_SetFilteredInet.
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_GetFilteredInetHitCount,
                             int /* hit_count */)

  // Is the browser in fullscreen mode?
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_IsFullscreen,
                             int /* browser_handle */,
                             bool /* is_fullscreen */)

  // Is the fullscreen bubble visible?
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_IsFullscreenBubbleVisible,
                             int /* browser_handle */,
                             bool /* is_visible */)

#if defined(OS_POSIX)
  // See previous definition of this message for explanation of why it is
  // defined twice.
  IPC_MESSAGE_ROUTED3(AutomationMsg_WindowClick, int, gfx::Point, int)
#endif

  // This message notifies the AutomationProvider to navigate to a specified
  // url in the tab with given handle. The first parameter is the handle to
  // the tab resource. The second parameter is the target url.  The third
  // parameter is the number of navigations that are required for a successful
  // return value. See AutomationMsg_NavigationResponseValues for the return
  // value.
  IPC_SYNC_MESSAGE_ROUTED3_1(
      AutomationMsg_NavigateToURLBlockUntilNavigationsComplete, int, GURL, int,
      AutomationMsg_NavigationResponseValues)

  // This message notifies the AutomationProvider to navigate to a specified
  // navigation entry index in the external tab with given handle. The first
  // parameter is the handle to the tab resource. The second parameter is the
  // index of navigation entry.
  // The return value contains a status code which is nonnegative on success.
  // see AutomationMsg_NavigationResponseValues for the navigation response.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_NavigateExternalTabAtIndex, int, int,
                             AutomationMsg_NavigationResponseValues)

  // This message requests the provider to wait until the window count
  // reached the specified value.
  // Request:
  //   - int: target browser window count
  // Response:
  //  - bool: whether the operation was successful.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_WaitForBrowserWindowCountToBecome,
                             int, bool)

  // This message requests the provider to wait until an application modal
  // dialog is shown.
  // Response:
  //  - bool: whether the operation was successful
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_WaitForAppModalDialogToBeShown, bool)

  // This message notifies the AutomationProvider to navigate back in session
  // history in the tab with given handle. The first parameter is the handle
  // to the tab resource. The second parameter is the number of navigations the
  // provider will wait for.
  // See AutomationMsg_NavigationResponseValues for the navigation response
  // values.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_GoBackBlockUntilNavigationsComplete,
                             int, int,
                             AutomationMsg_NavigationResponseValues)

  // This message notifies the AutomationProvider to navigate forward in session
  // history in the tab with given handle. The first parameter is the handle
  // to the tab resource. The second parameter is the number of navigations
  // the provider will wait for.
  // See AutomationMsg_NavigationResponseValues for the navigation response
  // values.
  IPC_SYNC_MESSAGE_ROUTED2_1(
      AutomationMsg_GoForwardBlockUntilNavigationsComplete, int, int,
      AutomationMsg_NavigationResponseValues)

  // This message is used by automation clients to upload histogram data to the
  // browser process.
  IPC_MESSAGE_ROUTED1(AutomationMsg_RecordHistograms,
                      std::vector<std::string> /* histogram_list */)

  IPC_MESSAGE_ROUTED2(AutomationMsg_AttachExternalTab,
                      int     /* 'source' tab_handle */,
                      IPC::AttachExternalTabParams)

  // Sent when the automation client connects to an existing tab.
  IPC_SYNC_MESSAGE_ROUTED2_3(AutomationMsg_ConnectExternalTab,
                             uint64 /* cookie */,
                             bool   /* allow/block tab*/,
                             gfx::NativeWindow  /* Tab container window */,
                             gfx::NativeWindow  /* Tab window */,
                             int  /* Handle to the new tab */)

#if defined(OS_POSIX)
  // TODO(estade): this should be merged with the windows message of the same
  // name. See comment for WindowClick.
  IPC_SYNC_MESSAGE_ROUTED4_1(AutomationMsg_WindowDrag,
                             int, std::vector<gfx::Point>, int, bool, bool)
#endif  // defined(OS_POSIX)

  // This message gets the bounds of the window.
  // Request:
  //   int - the handle of the window to query
  // Response:
  //   gfx::Rect - the bounds of the window
  //   bool - true if the query was successful
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_GetWindowBounds, int, gfx::Rect,
                             bool)

  // Simulate an end of session. Normally this happens when the user
  // shuts down the machine or logs off.
  // Request:
  //   int - the handle of the browser
  // Response:
  //   bool - true if succesful
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_TerminateSession, int, bool)

  // Returns whether the window is maximized.
  // Request:
  //   int - the handle of the window
  // Response:
  //   bool - true if the window is maximized
  //   bool - true if query is successful
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_IsWindowMaximized, int, bool, bool)

  IPC_MESSAGE_ROUTED2(AutomationMsg_SetPageFontSize,
                      int /* tab_handle */,
                      int /* The font size */)

  // Returns a metric event duration that was last recorded.  Returns -1 if the
  // event hasn't occurred yet.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_GetMetricEventDuration,
                             std::string /* event_name */,
                             int /* duration ms */)

  // Sent by automation provider - go to history entry via automation.
  IPC_MESSAGE_ROUTED2(AutomationMsg_RequestGoToHistoryEntryOffset,
                             int,   // tab handle
                             int)   // numbers of entries (negative or positive)

  // Silently install the extension in the given crx file.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_InstallExtension,
                             FilePath /* full path to crx file */,
                             AutomationMsg_ExtensionResponseValues)

  // Silently load the extension in the given directory.  This expects an
  // extension expanded into the directory, not a crx file.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_LoadExpandedExtension,
                             FilePath /* root directory of extension */,
                             AutomationMsg_ExtensionResponseValues)

  // Retrieves a list of the root directories of all enabled extensions
  // that have been installed into Chrome by dropping a .crx file onto
  // Chrome or an equivalent action.  Other types of extensions are not
  // included on the list (e.g. "component" extensions, "external"
  // extensions or extensions loaded via --load-extension since the first
  // two are generally not useful for testing (e.g. an external extension
  // could mess with an automated test if it's present on some systems only)
  // and the last would generally be explicitly loaded by tests.
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_GetEnabledExtensions,
                             std::vector<FilePath>)

  // This message requests the type of the window with the given handle. The
  // return value contains the type (Browser::Type), or -1 if the request
  // failed.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_Type, int, int)

  // Opens a new browser window of a specific type.
  IPC_SYNC_MESSAGE_ROUTED2_0(AutomationMsg_OpenNewBrowserWindowOfType,
                             int   /* Type (Browser::Type) */,
                             bool  /* show */ )

  // This message requests that the mouse be moved to this location, in
  // window coordinate space.
  // Request:
  //   int - the handle of the window that's the context for this click
  //   gfx::Point - the location to move to
  IPC_MESSAGE_ROUTED2(AutomationMsg_WindowMouseMove, int, gfx::Point)

  // Called when requests should be downloaded using a host browser's
  // download mechanism when chrome is being embedded.
  IPC_MESSAGE_ROUTED2(AutomationMsg_DownloadRequestInHost,
                      int /* tab_handle */,
                      int /* request_id */)

  // Shuts down the session service for the browser identified by
  // |browser_handle|. On success |result| is set to true.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_ShutdownSessionService,
                             int   /* browser_handle */,
                             bool  /* result */)

  IPC_MESSAGE_ROUTED1(AutomationMsg_SaveAsAsync,
                      int /* tab handle */)

#if defined(OS_WIN)
  // An incoming message from an automation host to Chrome.  Signals that
  // the browser containing |tab_handle| has moved.
  IPC_MESSAGE_ROUTED1(AutomationMsg_BrowserMove,
                      int /* tab handle */)
#endif

  // Used to get cookies for the given URL.
  IPC_MESSAGE_ROUTED3(AutomationMsg_GetCookiesFromHost,
                      int /* tab_handle */,
                      GURL /* url */,
                      int /* opaque_cookie_id */)

  IPC_MESSAGE_ROUTED5(AutomationMsg_GetCookiesHostResponse,
                      int /* tab_handle */,
                      bool /* success */,
                      GURL /* url */,
                      std::string /* cookies */,
                      int /* opaque_cookie_id */)

  // If the given host is empty, then the default content settings are
  // modified.
  IPC_SYNC_MESSAGE_ROUTED4_1(AutomationMsg_SetContentSetting,
                             int /* browser handle */,
                             std::string /* host */,
                             ContentSettingsType /* content type */,
                             ContentSetting /* setting */,
                             bool /* success */)

#if defined(OS_CHROMEOS)
  // Logs in through the browser's login wizard if available.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_LoginWithUserAndPass,
                             std::string /* username*/,
                             std::string /* password*/,
                             bool /* Whether successful*/)
#endif

  // Return the bookmarks encoded as a JSON string.
  IPC_SYNC_MESSAGE_ROUTED1_2(AutomationMsg_GetBookmarksAsJSON,
                             int /* browser_handle */,
                             std::string /* bookmarks as a JSON string */,
                             bool /* success */)

  // Wait for the bookmark model to load.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_WaitForBookmarkModelToLoad,
                             int /* browser_handle */,
                             bool /* success */)

  // Bookmark addition, modification, and removal.
  // Bookmarks are indexed by their id.
  IPC_SYNC_MESSAGE_ROUTED4_1(AutomationMsg_AddBookmarkGroup,
                             int /* browser_handle */,
                             int64 /* parent_id */,
                             int /* index */,
                             std::wstring /* title */,
                             bool /* success */)
  IPC_SYNC_MESSAGE_ROUTED5_1(AutomationMsg_AddBookmarkURL,
                             int /* browser_handle */,
                             int64 /* parent_id */,
                             int /* index */,
                             std::wstring /* title */,
                             GURL /* url */,
                             bool /* success */)
  IPC_SYNC_MESSAGE_ROUTED4_1(AutomationMsg_ReparentBookmark,
                             int /* browser_handle */,
                             int64 /* id */,
                             int64 /* new_parent_id */,
                             int /* index */,
                             bool /* success */)
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_SetBookmarkTitle,
                             int /* browser_handle */,
                             int64 /* id */,
                             std::wstring /* title */,
                             bool /* success */)
  IPC_SYNC_MESSAGE_ROUTED3_1(AutomationMsg_SetBookmarkURL,
                             int /* browser_handle */,
                             int64 /* id */,
                             GURL /* url */,
                             bool /* success */)
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_RemoveBookmark,
                             int /* browser_handle */,
                             int64 /* id */,
                             bool /* success */)

  // This message informs the browser process to remove the history entries
  // for the specified types across all time ranges. See
  // browsing_data_remover.h for a list of REMOVE_* types supported in the
  // remove_mask parameter.
  IPC_MESSAGE_ROUTED1(AutomationMsg_RemoveBrowsingData, int)

  // Block until the focused view id changes to something other than
  // |previous_view_id|.
  IPC_SYNC_MESSAGE_ROUTED2_2(AutomationMsg_WaitForFocusedViewIDToChange,
                             int /* window handle */,
                             int /* previous_view_id */,
                             bool /* success */,
                             int /* new_view_id */)

  // To avoid race conditions, waiting until a popup menu opens is a
  // three-step process:
  //   1. Call StartTrackingPopupMenus.
  //   2. Call an automation method that results in opening the popup menu.
  //   3. Call WaitForPopupMenuToOpen and check for success.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_StartTrackingPopupMenus,
                             int /* browser handle */,
                             bool /* success */)
  IPC_SYNC_MESSAGE_ROUTED0_1(AutomationMsg_WaitForPopupMenuToOpen,
                             bool /* success */)

  // Generic pyauto pattern to help avoid future addition of
  // automation messages.
  IPC_SYNC_MESSAGE_ROUTED2_2(AutomationMsg_SendJSONRequest,
                             int /* browser_handle */,
                             std::string /* JSON request */,
                             std::string /* JSON response */,
                             bool /* success */)

  // Installs an extension from the crx file and returns its id.
  // On error, |extension handle| will be 0.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_InstallExtensionAndGetHandle,
                             FilePath     /* full path to crx file */,
                             bool         /* with UI */,
                             int          /* extension handle */)

  // Waits for the next extension test result. Sets |test result| as the
  // received result and |message| as any accompanying message with the
  // result, which could be the empty string.
  IPC_SYNC_MESSAGE_ROUTED0_2(AutomationMsg_WaitForExtensionTestResult,
                             bool         /* test result */,
                             std::string  /* message */)

  // Uninstalls an extension. On success |success| is true.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_UninstallExtension,
                             int   /* extension handle */,
                             bool  /* success */)

  // Enables an extension. On success |success| is true.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_EnableExtension,
                             int   /* extension handle */,
                             bool  /* success */)

  // Disables an extension. On success |success| is true.
  IPC_SYNC_MESSAGE_ROUTED1_1(AutomationMsg_DisableExtension,
                             int   /* extension handle */,
                             bool  /* success */)

  // Executes the action associated with the given extension. This executes
  // the extension's page or browser action in the given browser, but does
  // not open popups. On success |success| is true.
  IPC_SYNC_MESSAGE_ROUTED2_1(
      AutomationMsg_ExecuteExtensionActionInActiveTabAsync,
      int   /* extension handle */,
      int   /* browser handle */,
      bool  /* success */)

  // Moves the browser action to the given index in the browser action toolbar.
  // On success |success| is true.
  IPC_SYNC_MESSAGE_ROUTED2_1(AutomationMsg_MoveExtensionBrowserAction,
                             int   /* extension handle */,
                             int   /* index */,
                             bool  /* success */)

  // Gets an extension property |property type|. On success |success| is true,
  // and |property value| is set.
  IPC_SYNC_MESSAGE_ROUTED2_2(AutomationMsg_GetExtensionProperty,
      int                              /* extension handle */,
      AutomationMsg_ExtensionProperty  /* property type */,
      bool                             /* success */,
      std::string                      /* property value */)

  // Resets to the default theme.
  IPC_SYNC_MESSAGE_ROUTED0_0(AutomationMsg_ResetToDefaultTheme)

IPC_END_MESSAGES(Automation)
