#ifndef LIB_YANDEX_DEF
#define LIB_YANDEX_DEF

typedef struct LuaCallbackInfo
{
    int m_L;
    int        m_ContextTableRef;
    int        m_CallbackInfoRef;
    int        m_Callback;
    int        m_Self;
} LuaCallbackInfo;

#pragma region Features

extern void JS_LoadingAPI_Ready();
extern void JS_GameplayAPI_Start();
extern void JS_GameplayAPI_Stop();

// ===============================================
// Games API
// ===============================================

typedef void (*GetAllGamesHandler)(LuaCallbackInfo *callback,
                                      const int success, const char *json);

typedef void (*GetGameByIDHandler)(LuaCallbackInfo *callback,
                                      const int success, const char *json);

extern void JS_GamesAPI_GetAllGames(GetAllGamesHandler handler, LuaCallbackInfo *callback);
extern void JS_GamesAPI_GetGameByID(GetGameByIDHandler handler, LuaCallbackInfo *callback, const int id);

#pragma endregion

#pragma region Payments

// ===============================================
// Create Purchase
// ===============================================

typedef void (*CreatePurchaseHandler)(LuaCallbackInfo *callback,
                                      const int success, const char *json,
                                      const char *signature);


extern void JS_CreatePurchase(CreatePurchaseHandler handler,
                       LuaCallbackInfo *callback, const char *params);

// ===============================================
// Open Auth Dialog
// ===============================================

typedef void (*OpenAuthDialogHandler)(LuaCallbackInfo *callback,
                                      const int success);


extern void JS_OpenAuthDialog(OpenAuthDialogHandler handler,
                       LuaCallbackInfo *callback);

// ===============================================
// Get Purchases
// ===============================================

typedef void (*GetPurchasesHandler)(LuaCallbackInfo *callback,
                                    const int success, const char *json,
                                    const char *signature);


extern void JS_GetPurchases(GetPurchasesHandler handler,
                     LuaCallbackInfo *callback, const char *params);

// ===============================================
// Get Catalog
// ===============================================

typedef void (*GetCatalogHandler)(LuaCallbackInfo *callback,
                                  const int success, const char *jsons);


extern void JS_GetCatalog(GetCatalogHandler handler,
                   LuaCallbackInfo *callback);

// ===============================================
// Consume Purchase
// ===============================================

typedef void (*ConsumePurchaseHandler)(LuaCallbackInfo *callback,
                                       const int success);


extern void JS_ConsumePurchase(ConsumePurchaseHandler handler,
                        LuaCallbackInfo *callback,
                        const char *purchaseToken);

#pragma endregion

#pragma region Player

// ===============================================
// Get Player Info
// ===============================================

typedef void (*GetPlayerInfoHandler)(LuaCallbackInfo *callback,
                                     const int success, const char *json,
                                     const char *signature);


extern void JS_GetPlayerInfo(GetPlayerInfoHandler handler,
                      LuaCallbackInfo *callback, const char *params);

// ===============================================
// Get Player IDs Per Game
// ===============================================

typedef void (*GetPlayerIDsPerGameHandler)(LuaCallbackInfo *callback,
                                           const int success, const char *json);


extern void JS_GetPlayerIDsPerGame(GetPlayerIDsPerGameHandler handler,
                            LuaCallbackInfo *callback);

// ===============================================
// Get Player Data
// ===============================================

typedef void (*GetPlayerDataHandler)(LuaCallbackInfo *callback,
                                     const int success, const char *json);


extern void JS_GetPlayerData(GetPlayerDataHandler handler,
                      LuaCallbackInfo *callback, const char *keys);

// ===============================================
// Set Player Data
// ===============================================


extern void JS_SetPlayerData(const char *data, const int flush);

// ===============================================
// Get Player Stats
// ===============================================

typedef void (*GetPlayerStatsHandler)(LuaCallbackInfo *callback,
                                      const int success, const char *json);


extern void JS_GetPlayerStats(GetPlayerStatsHandler handler,
                       LuaCallbackInfo *callback, const char *keys);

// ===============================================
// Set Player Stats
// ===============================================


extern void JS_SetPlayerStats(const char *stats);

// ===============================================
// Increment Player Stats
// ===============================================


extern void JS_IncrementPlayerStats(const char *increments);

#pragma endregion

#pragma region Feedback

// ===============================================
// Can Review
// ===============================================

typedef void (*CanReviewHandler)(LuaCallbackInfo *callback,
                                 const int success, const int canReview,
                                 const char *reason);


extern void JS_CanReview(CanReviewHandler handler,
                  LuaCallbackInfo *callback);

// ===============================================
// Request Review
// ===============================================

typedef void (*RequestReviewHandler)(LuaCallbackInfo *callback,
                                     const int success, const int feedbackSent);


extern void JS_RequestReview(RequestReviewHandler handler,
                      LuaCallbackInfo *callback);

#pragma endregion

#pragma region Environment


extern char *JS_GetEnvironment();

#pragma endregion

#pragma region Adv

// ===============================================
// Fullscreen Adv
// ===============================================

typedef void (*ShowFullscreenAdvHandler)(LuaCallbackInfo *onOpen,
                                         LuaCallbackInfo *onClose,
                                         LuaCallbackInfo *onError,
                                         LuaCallbackInfo *onOffline,
                                         const int callbackType,
                                         const char *data);


extern void JS_ShowFullscreenAdv(ShowFullscreenAdvHandler handler,
                          LuaCallbackInfo *onOpen,
                          LuaCallbackInfo *onClose,
                          LuaCallbackInfo *onError,
                          LuaCallbackInfo *onOffline);

// ===============================================
// Rewarded Video
// ===============================================

typedef void (*ShowRewardedVideoHandler)(LuaCallbackInfo *onOpen,
                                         LuaCallbackInfo *onRewarded,
                                         LuaCallbackInfo *onClose,
                                         LuaCallbackInfo *onError,
                                         const int callbackType,
                                         const char *data);


extern void JS_ShowRewardedVideo(ShowRewardedVideoHandler handler,
                          LuaCallbackInfo *onOpen,
                          LuaCallbackInfo *onRewarded,
                          LuaCallbackInfo *onClose,
                          LuaCallbackInfo *onError);

// ===============================================
// Sticky Banner
// ===============================================

typedef void (*GetBannerAdvStatusHandler)(LuaCallbackInfo *callback,
                                          const int success,
                                          const int stickyAdvIsShowing,
                                          const char *reason);

extern void JS_GetBannerAdvStatus(GetBannerAdvStatusHandler handler,
                           LuaCallbackInfo *callback);
extern void JS_ShowBannerAdv();
extern void JS_HideBannerAdv();

#pragma endregion

#pragma region Events

typedef void (*CallEventCallback)(LuaCallbackInfo *callback);
typedef void (*DestroyEventCallback)(LuaCallbackInfo *callback);

extern void JS_InitEvents(CallEventCallback callEventCallback, DestroyEventCallback destroyEventCallback);
extern void JS_OnEvent(const char *eventName, const void *pointer, LuaCallbackInfo *callback);
extern void JS_OffEvent(const char *eventName, const void *pointer);
extern void JS_DispatchEvent(const char *eventName, const char *detail);

#pragma endregion

#pragma region Leaderboards

// ===============================================
// Get Leaderboard Description
// ===============================================

typedef void (*GetLeaderboardDescriptionHandler)(
    LuaCallbackInfo *callback, const int success, const char *json);

extern void JS_GetLeaderboardDescription(GetLeaderboardDescriptionHandler handler,
                                  LuaCallbackInfo *callback,
                                  const char *leaderboardName);

// ===============================================
// Set Leaderboard Score
// ===============================================

extern void JS_SetLeaderboardScore(const char *leaderboardName, const char *score,
                            const char *extraData);

// ===============================================
// Get Leaderboard Player Entry
// ===============================================

typedef void (*GetLeaderboardPlayerEntryHandler)(
    LuaCallbackInfo *callback, const int success, const char *json);

extern void JS_GetLeaderboardPlayerEntry(GetLeaderboardPlayerEntryHandler handler,
                                  LuaCallbackInfo *callback,
                                  const char *leaderboardName);

// ===============================================
// Get Leaderboard Entries
// ===============================================

typedef void (*GetLeaderboardEntriesHandler)(
    LuaCallbackInfo *callback, const int success, const char *json);

extern void JS_GetLeaderboardEntries(GetLeaderboardPlayerEntryHandler handler,
                              LuaCallbackInfo *callback,
                              const char *leaderboardName, const char *params);

#pragma endregion

#pragma region Shortcut

// ===============================================
// Can Show Prompt
// ===============================================

typedef void (*CanShowPromptHandler)(LuaCallbackInfo *callback,
                                     const int success,
                                     const int canShowPrompt);

extern void JS_CanShowPrompt(CanShowPromptHandler handler,
                      LuaCallbackInfo *callback);

// ===============================================
// Show Prompt
// ===============================================

typedef void (*ShowPromptHandler)(LuaCallbackInfo *callback,
                                  const int success, const int accepted);

extern void JS_ShowPrompt(ShowPromptHandler handler,
                   LuaCallbackInfo *callback);

#pragma endregion

#pragma region Screen

// ===============================================
// Fullscreen
// ===============================================

extern char *JS_Screen_Fullscreen_GetStatus();
extern void JS_Screen_Fullscreen_Request();
extern void JS_Screen_Fullscreen_Exit();

// ===============================================
// Orientation
// ===============================================

extern char *JS_Screen_Orientation_Get();
extern void JS_Screen_Orientation_Set(const char *value);

#pragma endregion

#pragma region Clipboard

extern void JS_ClipboardWriteText(const char *text);

#pragma endregion

#pragma region DeviceInfo

extern char *JS_GetDeviceInfo();

#pragma endregion

#pragma region Flags

// ===============================================
// Get Flags
// ===============================================

typedef void (*GetFlagsHandler)(LuaCallbackInfo *callback,
                                const int success, const char *json);

extern void JS_GetFlags(GetFlagsHandler handler, LuaCallbackInfo *callback,
                 const char *params);

#pragma endregion

#pragma region Server Time

extern double JS_GetServerTime();

#pragma endregion

#endif //LIB_YANDEX_DEF