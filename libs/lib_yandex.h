#ifndef LIB_YANDEX_DEF
#define LIB_YANDEX_DEF

#pragma region Features

extern void JS_LoadingAPI_Ready();
extern void JS_GameplayAPI_Start();
extern void JS_GameplayAPI_Stop();

// ===============================================
// Games API
// ===============================================

typedef void (*GetAllGamesHandler)(const int success, const char *json);

typedef void (*GetGameByIDHandler)(const int success, const char *json);

extern void JS_GamesAPI_GetAllGames(GetAllGamesHandler handler);
extern void JS_GamesAPI_GetGameByID(GetGameByIDHandler handler, const int id);

#pragma endregion

#pragma region Payments

// ===============================================
// Create Purchase
// ===============================================

typedef void (*CreatePurchaseHandler)(const int success, const char *json,
                                      const char *signature);


extern void JS_CreatePurchase(CreatePurchaseHandler handler, const char *params);

// ===============================================
// Open Auth Dialog
// ===============================================

typedef void (*OpenAuthDialogHandler)(const int success);


extern void JS_OpenAuthDialog(OpenAuthDialogHandler handler);

// ===============================================
// Get Purchases
// ===============================================

typedef void (*GetPurchasesHandler)(const int success, const char *json,
                                    const char *signature);


extern void JS_GetPurchases(GetPurchasesHandler handler, const char *params);

// ===============================================
// Get Catalog
// ===============================================

typedef void (*GetCatalogHandler)(const int success, const char *jsons);


extern void JS_GetCatalog(GetCatalogHandler handler);

// ===============================================
// Consume Purchase
// ===============================================

typedef void (*ConsumePurchaseHandler)( const int success);


extern void JS_ConsumePurchase(ConsumePurchaseHandler handler,
                        const char *purchaseToken);

#pragma endregion

#pragma region Player

// ===============================================
// Get Player Info
// ===============================================

typedef void (*GetPlayerInfoHandler)( const int success, const char *json,
                                     const char *signature);


extern void JS_GetPlayerInfo(GetPlayerInfoHandler handler, const char *params);

// ===============================================
// Get Player IDs Per Game
// ===============================================

typedef void (*GetPlayerIDsPerGameHandler)(     const int success, const char *json);


extern void JS_GetPlayerIDsPerGame(GetPlayerIDsPerGameHandler handler);

// ===============================================
// Get Player Data
// ===============================================

typedef void (*GetPlayerDataHandler)( const int success, const char *json);


extern void JS_GetPlayerData(GetPlayerDataHandler handler, const char *keys);

// ===============================================
// Set Player Data
// ===============================================


extern void JS_SetPlayerData(const char *data, const int flush);

// ===============================================
// Get Player Stats
// ===============================================

typedef void (*GetPlayerStatsHandler)(const int success, const char *json);


extern void JS_GetPlayerStats(GetPlayerStatsHandler handler, const char *keys);

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

typedef void (*CanReviewHandler)(const int success, const int canReview,
                                 const char *reason);


extern void JS_CanReview(CanReviewHandler handler);

// ===============================================
// Request Review
// ===============================================

typedef void (*RequestReviewHandler)( const int success, const int feedbackSent);


extern void JS_RequestReview(RequestReviewHandler handler);

#pragma endregion

#pragma region Environment


extern char *JS_GetEnvironment();

#pragma endregion

#pragma region Adv

// ===============================================
// Fullscreen Adv
// ===============================================

typedef void (*ShowFullscreenAdvHandler)(const int callbackType, const char *data);


extern void JS_ShowFullscreenAdv(ShowFullscreenAdvHandler handler);

// ===============================================
// Rewarded Video
// ===============================================

typedef void (*ShowRewardedVideoHandler)(const int callbackType,
                                         const char *data);


extern void JS_ShowRewardedVideo(ShowRewardedVideoHandler handler);

// ===============================================
// Sticky Banner
// ===============================================

typedef void (*GetBannerAdvStatusHandler)(    const int success,
                                          const int stickyAdvIsShowing,
                                          const char *reason);

extern void JS_GetBannerAdvStatus(GetBannerAdvStatusHandler handler);
extern void JS_ShowBannerAdv();
extern void JS_HideBannerAdv();

#pragma endregion

#pragma region Events

typedef void (*CallEventCallback)();
typedef void (*DestroyEventCallback)();

extern void JS_InitEvents(CallEventCallback callEventCallback, DestroyEventCallback destroyEventCallback);
extern void JS_OnEvent(const char *eventName, const void *pointer);
extern void JS_OffEvent(const char *eventName, const void *pointer);
extern void JS_DispatchEvent(const char *eventName, const char *detail);

#pragma endregion

#pragma region Leaderboards

// ===============================================
// Get Leaderboard Description
// ===============================================

typedef void (*GetLeaderboardDescriptionHandler)(const int success, const char *json);

extern void JS_GetLeaderboardDescription(GetLeaderboardDescriptionHandler handler,
                                  const char *leaderboardName);

// ===============================================
// Set Leaderboard Score
// ===============================================

extern void JS_SetLeaderboardScore(const char *leaderboardName, const char *score,
                            const char *extraData);

// ===============================================
// Get Leaderboard Player Entry
// ===============================================

typedef void (*GetLeaderboardPlayerEntryHandler)(const int success, const char *json);

extern void JS_GetLeaderboardPlayerEntry(GetLeaderboardPlayerEntryHandler handler,
                                  const char *leaderboardName);

// ===============================================
// Get Leaderboard Entries
// ===============================================

typedef void (*GetLeaderboardEntriesHandler)(const int success, const char *json);

extern void JS_GetLeaderboardEntries(GetLeaderboardPlayerEntryHandler handler,
                              const char *leaderboardName, const char *params);

#pragma endregion

#pragma region Shortcut

// ===============================================
// Can Show Prompt
// ===============================================

typedef void (*CanShowPromptHandler)( const int success,
                                     const int canShowPrompt);

extern void JS_CanShowPrompt(CanShowPromptHandler handler);

// ===============================================
// Show Prompt
// ===============================================

typedef void (*ShowPromptHandler)(const int success, const int accepted);

extern void JS_ShowPrompt(ShowPromptHandler handler);

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

typedef void (*GetFlagsHandler)(const int success, const char *json);

extern void JS_GetFlags(GetFlagsHandler handler,
                 const char *params);

#pragma endregion

#pragma region Server Time

extern double JS_GetServerTime();

#pragma endregion

#endif //LIB_YANDEX_DEF