let LisGamesSDKLib = {
  $YGTigor: {
    allocateString: function (str) {
      const size = lengthBytesUTF8(str) + 1;
      const idx = _malloc(size);
      stringToUTF8Array(str, HEAP8, idx, size);
      return idx;
    },

    allocateJSON: function (str) {
      return YGTigor.allocateString(JSON.stringify(str));
    },
  },

//#region Features

JS_LoadingAPI_Ready: function () {
  if (window.ysdk.features.LoadingAPI) {
    window.ysdk.features.LoadingAPI.ready();
  }
},

JS_GameplayAPI_Start: function () {
  if (window.ysdk.features.GameplayAPI) {
    window.ysdk.features.GameplayAPI.start();
  }
},

JS_GameplayAPI_Stop: function () {
  if (window.ysdk.features.GameplayAPI) {
    window.ysdk.features.GameplayAPI.stop();
  }
},

JS_GamesAPI_GetAllGames: function (handler) {
  if (window.ysdk.features.GamesAPI) {
    window.ysdk.features.GamesAPI.getAllGames()
      .then(function (result) {
        {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(result))
      })
      .catch(function () {
        {{{ makeDynCall('viii', 'handler') }}}(0, 0)
      });
  }
},

JS_GamesAPI_GetGameByID: function (handler, id) {
  if (window.ysdk.features.GamesAPI) {
    window.ysdk.features.GamesAPI.getGameByID(id)
      .then(function (result) {
        {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(result))
      })
      .catch(function () {
        {{{ makeDynCall('viii', 'handler') }}}(0, 0)
      });
  }
},

//#endregion

//#region Payments

JS_CreatePurchase: function (handler, cparams) {
  const params = JSON.parse(UTF8ToString(cparams));

  window.ysdk.getPayments({ signed: params.signed ?? false })
    .then(function (payments) {
      return payments.purchase({ 
        id: params.id,
        developerPayload: params.developer_payload ?? ''
      });
    })
    .then(function (purchase) {
      const cpurchase = YGTigor.allocateJSON({
        product_id: purchase.productID,
        purchase_token: purchase.purchaseToken,
        developer_payload: purchase.developerPayload,
      });
      const csignature = YGTigor.allocateString(purchase.signature ?? '');
      {{{ makeDynCall('viii', 'handler') }}}(1, cpurchase, csignature)
    })
    .catch(function () {
      {{{ makeDynCall('viii', 'handler') }}}(0, 0, 0)
    });
},

JS_GetPurchases: function (handler, cparams) {
  const params = JSON.parse(UTF8ToString(cparams));

  window.ysdk.getPayments({ signed: params.signed ?? false })
    .then(function (payments) {
      return payments.getPurchases();
    })
    .then(function (purchases) {
      const cpurchase = YGTigor.allocateJSON(purchases.map(function (purchase) {
        return {
          product_id: purchase.productID,
          purchase_token: purchase.purchaseToken,
          developer_payload: purchase.developerPayload,
        };
      }));
      const csignature = YGTigor.allocateString(purchases.signature ?? '');
      {{{ makeDynCall('viii', 'handler') }}}(1, cpurchase, csignature)
    })
    .catch(function () {
      {{{ makeDynCall('viii', 'handler') }}}(0, 0, 0)
    });
},

JS_GetCatalog: function (handler) {
  window.ysdk.getPayments()
    .then(function (payments) {
      return payments.getCatalog();
    })
    .then(function (purchases) {
      const ccatalog = YGTigor.allocateJSON(purchases.map(function (product) {
        return {
          id: product.id,
          title: product.title,
          description: product.description,
          image_uri: product.image_uri,
          price: product.price,
          price_value: product.price_value,
          price_currency_code: product.price_currency_code,
          price_currency_image: {
            small: product.getPriceCurrencyImage("small"),
            medium: product.getPriceCurrencyImage("medium"),
            svg: product.getPriceCurrencyImage("svg"),
          }
        }
      }));
      {{{ makeDynCall('vii', 'handler') }}}(1, ccatalog)
    })
    .catch(function () {
      {{{ makeDynCall('vii', 'handler') }}}(0, 0)
    });
},

JS_ConsumePurchase: function (handler, cpurchaseToken) {
  const purchaseToken = UTF8ToString(cpurchaseToken);

  window.ysdk.getPayments()
    .then(function (payments) {
      return payments.consumePurchase(purchaseToken);
    }).then(function () {
      {{{ makeDynCall('v', 'handler') }}}(1)
    }).catch(function () {
      {{{ makeDynCall('v', 'handler') }}}(0)
    });
},

//#endregion

//#region Player

JS_GetPlayerInfo: function (handler, cparams) {    
  const params = cparams ? JSON.parse(UTF8ToString(cparams)) : undefined;

  window.ysdk.getPlayer({
    signed: params?.signed ?? false,
    scopes: params?.scopes ?? false
  })
    .then(function (player) {
      const cplayerInfo = YGTigor.allocateJSON({
        logged_in: player.getMode() !== 'lite',
        unique_id: player.getUniqueID() ?? "",
        name: player.getName() ?? "",
        photo: {
          small: player.getPhoto('small') ?? "",
          medium: player.getPhoto('medium') ?? "",
          large: player.getPhoto('large') ?? "",
        },
        paying_status: player.getPayingStatus() ?? "",
      });

      const csignature = YGTigor.allocateString(player.signature ?? '');

      {{{ makeDynCall('viii', 'handler') }}}(1, cplayerInfo, csignature)
    })
    .catch(function () {
      {{{ makeDynCall('viii', 'handler') }}}(0, 0, 0)
    });
},

JS_OpenAuthDialog: function (handler) {
  window.ysdk.auth.openAuthDialog()
    .then(function () {
      {{{ makeDynCall('vi', 'handler') }}}(1)
    })
    .catch(function () {
      {{{ makeDynCall('vi', 'handler') }}}(0)
    });
},

JS_GetPlayerIDsPerGame: function (handler) {
  window.ysdk.isAvailableMethod('player.getIDsPerGame').then(function (available) {
    if (available) {
      window.ysdk.getPlayer()
        .then(function (player) {
          return player.getIDsPerGame();
        })
        .then(function (ids) {
          {{{ makeDynCall('viii', 'handler') }}}(
            callback,
             1, 
            YGTigor.allocateJSON(
              ids.map(function (id) {
                return {
                  app_id: id.appID,
                  user_id: id.userID
                }
              })
            )
          )
        })
        .catch(function () {
          {{{ makeDynCall('vii', 'handler') }}}(0, 0)
        });
    } else {
      {{{ makeDynCall('vii', 'handler') }}}(0, 0)
    }
  }).catch(function () {
    {{{ makeDynCall('vii', 'handler') }}}(0, 0)
  });
},

JS_GetPlayerData: function (handler, ckeys) {
  let keys;

  if (ckeys) {
    keys = JSON.parse(UTF8ToString(ckeys));
  }
  
  window.ysdk.getPlayer()
    .then(function (player) {
      return player.getData(keys);
    })
    .then(function (data) {
      {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(data))
    })
    .catch(function () {
      {{{ makeDynCall('viii', 'handler') }}}(0, 0)
    });
},

JS_SetPlayerData: function (cdata, cflush) {
  const data = JSON.parse(UTF8ToString(cdata));
  const flush = cflush == 1;
  
  window.ysdk
    .getPlayer()
    .then(function (player) {
      player.setData(data, flush);
    });
},

JS_GetPlayerStats: function (handler, ckeys) {
  let keys;

  if (ckeys) {
    keys = JSON.parse(UTF8ToString(ckeys));
  }
  
  window.ysdk
    .getPlayer()
    .then(function (player) {
      return player.getStats(keys);
    })
    .then(function (stats) {
      {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(stats))
    })
    .catch(function () {
      {{{ makeDynCall('vii', 'handler') }}}(0, 0)
    });
},

JS_IncrementPlayerStats: function (cstats) {
  const stats = JSON.parse(UTF8ToString(cstats));
  
  window.ysdk
    .getPlayer()
    .then(function (player) {
      player.setStats(stats);
    });
},

JS_SetPlayerStats: function (cstats) {
  const stats = JSON.parse(UTF8ToString(cstats));
  
  window.ysdk
    .getPlayer()
    .then(function (player) {
      player.setStats(stats);
    });
},

//#endregion

//#region Feedback

JS_CanReview: function (handler) {
  ysdk.feedback
    .canReview()
    .then(function (response) {
        {{{ makeDynCall('viii', 'handler') }}}(1, response.value, YGTigor.allocateString(repsonse.reason ?? ''))
    })
    .catch(function () {
      {{{ makeDynCall('viii', 'handler') }}}(0, 0, YGTigor.allocateString(''))
    });
},

JS_RequestReview: function (handler) {
  ysdk.feedback
    .requestReview()
    .then(function (response) {
        {{{ makeDynCall('vii', 'handler') }}}(1, response.feedbackSent)
    })
    .catch(function () {
      {{{ makeDynCall('vii', 'handler') }}}(0, 0)
    });
},

//#endregion

//#region Environment

JS_GetEnvironment: function () {
  const environment = {
    app: window.ysdk.environment.app,
    browser: window.ysdk.environment.browser,
    i18n: window.ysdk.environment.i18n,
    payload: window.ysdk.environment.payload,  
  };
  return YGTigor.allocateString(JSON.stringify(environment));
},

//#endregion

//#region Adv

JS_ShowFullscreenAdv: function (handler) {
  window.ysdk.adv.showFullscreenAdv({
      callbacks: {
          onOpen: function() {
            {{{ makeDynCall('vii', 'handler') }}}(0, YGTigor.allocateString(''));
          },
          onClose: function(wasShown) {
            {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(wasShown));
          },
          onError: function(error) {
            {{{ makeDynCall('vii', 'handler') }}}(2, YGTigor.allocateString(error.toString()));
          },
          onOffline: function() {
            {{{ makeDynCall('vii', 'handler') }}}(3, YGTigor.allocateString(''));
          }
      }
  });
},

JS_ShowRewardedVideo: function (handler) {   
  window.ysdk.adv.showRewardedVideo({
      callbacks: {
          onOpen: function() {
            {{{ makeDynCall('vii', 'handler') }}}(0, YGTigor.allocateString(''));
          },
          onRewarded: function() {
            {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateString(''));
          },
          onClose: function() {
            {{{ makeDynCall('vii', 'handler') }}}(2, YGTigor.allocateString(''));
          },
          onError: function(error) { 
            {{{ makeDynCall('vii', 'handler') }}}(3, YGTigor.allocateString(error.toString()));
          }
      }
  });
},

JS_GetBannerAdvStatus: function(handler) {
  ysdk.adv.getBannerAdvStatus()
    .then(function (response) {
      {{{ makeDynCall('vii', 'handler') }}}(response.stickyAdvIsShowing, YGTigor.allocateString(response.reason ?? ''));
    })
    .catch(function () {
      {{{ makeDynCall('vii', 'handler') }}}(0, YGTigor.allocateString(''));
    });
},

JS_ShowBannerAdv: function () {
  ysdk.adv.showBannerAdv();
},

JS_HideBannerAdv: function () {
  ysdk.adv.hideBannerAdv();
},

//#endregion

//#region Events

JS_InitEvents: function (callEventdestroyEventCallback) {
  const EVENTS = {
    "GAME_API_PAUSE": new Map(),
    "GAME_API_RESUME": new Map(),
    "EXIT": new Map(),
    "HISTORY_BACK": new Map()
  };

  YGTigor.events = {
    checkEventName: function (eventName) {
      const handlers = EVENTS[eventName];
  
      if (handlers === undefined) {
        console.error(`Unknown event name ${eventName}. Acceptable values: ${Object.keys(EVENTS).join(", ")}.`)
        return false;
      }

      return true;
    },

    registerCallback: function (eventName) {
      EVENTS[eventName].set();
    },

    destroyCallback: function (eventName) {
      const handlers = EVENTS[eventName];


      {{{ makeDynCall('v', 'destroyEventCallback') }}};

    },
  }

  ysdk.on("game_api_pause", function () {
    for (const [] of EVENTS["GAME_API_PAUSE"]) {
      {{{ makeDynCall('v', 'callEventCallback') }}};
    }
  });

  ysdk.on("game_api_resume", function () {
    for (const [] of EVENTS["GAME_API_RESUME"]) {
      {{{ makeDynCall('v', 'callEventCallback') }}};
    }
  });

  ysdk.on("EXIT", function () {
    for (const [] of EVENTS["EXIT"]) {
      {{{ makeDynCall('v', 'callEventCallback') }}};
    }
  });

  ysdk.on("HISTORY_BACK", function () {
    for (const [] of EVENTS["HISTORY_BACK"]) {
      {{{ makeDynCall('v', 'callEventCallback') }}};
    }
  });
},

JS_OnEvent: function (ceventName) {
  const eventName = UTF8ToString(ceventName);

  if (YGTigor.events.checkEventName(eventName)) {
    YGTigor.events.registerCallback(eventName)
  }
},

JS_OffEvent: function (ceventName) {
  const eventName = UTF8ToString(ceventName);

  if (YGTigor.events.checkEventName(eventName)) {
    YGTigor.events.destroyCallback(eventName);
  }
},

JS_DispatchEvent: function (ceventName, cdetail) {
  const eventName = UTF8ToString(ceventName);
  const detail = JSON.parse(UTF8ToString(cdetail));
  ysdk.dispatchEvent(eventName, detail);
},

//#endregion

//#region Leaderboard

JS_GetLeaderboardDescription: function (handler, cleaderboardName) {
  const leaderboardName = UTF8ToString(cleaderboardName);
  
  window.ysdk.getLeaderboards()
    .then(function (lb) {
      return lb.getLeaderboardDescription(leaderboardName);
    })
    .then(function (res) {
      const description = {
        app_id: res.appID,
        default: res.default,
        invert_sort_order: res.description.invert_sort_order,
        decimal_offset: res.description.score_format.options.decimal_offset,
        type: res.description.type,
        name: res.name,
        title: res.title,  
      };
      console.log(res,description)
      {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(description))
    })
    .catch(function () {
      {{{ makeDynCall('vii', 'handler') }}}(0, 0)
    });
},

JS_SetLeaderboardScore: function (cleaderboardName, cscore, cextraData) {
  const leaderboardName = UTF8ToString(cleaderboardName);
  const score = parseFloat(UTF8ToString(cscore));
  const extraData = UTF8ToString(cextraData) ?? undefined;

  window.ysdk.getLeaderboards()
    .then(function (lb) {
      return lb.setLeaderboardScore(leaderboardName, score, extraData);
    });
},

JS_GetLeaderboardPlayerEntry: function (handler, cleaderboardName) {
  const leaderboardName = UTF8ToString(cleaderboardName);
  
  window.ysdk.getLeaderboards()
    .then(function (lb) {
      return lb.getLeaderboardPlayerEntry(leaderboardName);
    })
    .then(function (res) {
      const player_entry = {
        score: res.score,
        extraData: res.extraData,
        rank: res.rank,
        avatar_src: {
          small: res.player.getAvatarSrc("small"),
          medium: res.player.getAvatarSrc("medium"),
          large: res.player.getAvatarSrc("large"),
        },
        avatar_srcset: {
          small: res.player.getAvatarSrcSet("small"),
          medium: res.player.getAvatarSrcSet("medium"),
          large: res.player.getAvatarSrcSet("large"),
        },
        lang: res.player.lang,
        public_name: res.player.public_name,
        unique_id: res.player.unique_id,
        formattedScore: res.formattedScore,
      }
      {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(player_entry))
    })
    .catch(function () {
      {{{ makeDynCall('vii', 'handler') }}}(0, 0)
    });
},

JS_GetLeaderboardEntries: function (handler, cleaderboardName, cparams) {
  const leaderboardName = UTF8ToString(cleaderboardName);
  const params = cparams ? JSON.parse(UTF8ToString(cparams)) : undefined;

  window.ysdk.getLeaderboards()
    .then(function (lb) {
      console.log("3", lb,  {
        includeUser: params?.include_user ?? false,
        quantityAround: params?.quantity_around ?? 5,
        quantityTop: params?.quantity_top ?? 5,
      })
      return lb.getLeaderboardEntries(
        leaderboardName,
        {
          includeUser: params?.include_user ?? false,
          quantityAround: params?.quantity_around ?? 5,
          quantityTop: params?.quantity_top ?? 5,
        }
      );
    })
    .then(function (res) {
      console.log("4", res)
      const entries = {
        leaderboard: {
          app_id: res.leaderboard.app_id,
          default: res.leaderboard.default,
          invert_sort_order: res.leaderboard.description.invert_sort_order,
          decimal_offset: res.leaderboard.description.score_format.options.decimal_offset,
          type: res.leaderboard.description.type,
          name: res.leaderboard.description.name,
          title: res.leaderboard.description.title,  
        },
        ranges: res.ranges,
        user_rank: res.userRank,
        entries: res.entries.map(function (entry) {
          return {
            score: entry.score,
            extraData: entry.extraData,
            rank: entry.rank,
            avatar_src: {
              small: entry.player.getAvatarSrc("small"),
              medium: entry.player.getAvatarSrc("medium"),
              large: entry.player.getAvatarSrc("large"),
            },
            avatar_srcset: {
              small: entry.player.getAvatarSrcSet("small"),
              medium: entry.player.getAvatarSrcSet("medium"),
              large: entry.player.getAvatarSrcSet("large"),
            },
            lang: entry.player.lang,
            public_name: entry.player.publicName,
            unique_id: entry.player.uniqueID,
            formattedScore: entry.formattedScore,
          };
        }),
      }
      console.log("5", entries);
      {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(entries))
    })
    .catch(function () {
      {{{ makeDynCall('vii', 'handler') }}}(0, 0)
    });
  },

  //#endregion

  //#region Shortcut

  JS_CanShowPrompt: function (handler) {
    ysdk.shortcut
      .canShowPrompt()
      .then(function (prompt) {
          {{{ makeDynCall('vi', 'handler') }}}(1, prompt.canShow)
      })
      .catch(function () {
        {{{ makeDynCall('vi', 'handler') }}}(0, 0)
      });
  },

  JS_ShowPrompt: function (handler) {
    ysdk.shortcut
      .showPrompt()
      .then(function (result) {
          {{{ makeDynCall('vi', 'handler') }}}(1, result.outcome === 'accepted')
      })
      .catch(function () {
        {{{ makeDynCall('vi', 'handler') }}}(0, 0)
      });
  },

  //#endregion

  //#region Screen

  JS_Screen_Fullscreen_GetStatus: function() {
    return YGTigor.allocateString(window.ysdk.screen.fullscreen.status);
  },

  JS_Screen_Fullscreen_Request: function() {
    window.ysdk.screen.fullscreen.request();
  },

  JS_Screen_Fullscreen_Exit: function() {
    window.ysdk.screen.fullscreen.exit();
  },

  JS_Screen_Orientation_Get: function() {
    return YGTigor.allocateString(window.ysdk.screen.orientation.value);
  },

  JS_Screen_Orientation_Set: function(cvalue) {
    window.ysdk.screen.orientation.value = UTF8ToString(cvalue);
  },

  //#endregion

  //#region Clipboard

  JS_ClipboardWriteText: function(ctext) {
    const text = UTF8ToString(ctext);
    window.ysdk.clipboard.writeText(text);
  },

  //#endregion

  //#region Device Info

  JS_GetDeviceInfo: function () {
    const deviceInfo = {
      type: window.ysdk.deviceInfo.type,
      isMobile: window.ysdk.deviceInfo.type === 'mobile',
      isDesktop: window.ysdk.deviceInfo.type === 'desktop',
      isTablet: window.ysdk.deviceInfo.type === 'tablet',
      isTV: window.ysdk.deviceInfo.type === 'tv',
    };
    return YGTigor.allocateString(JSON.stringify(deviceInfo));
  },

  //#endregion

  //#region Flags

  JS_GetFlags: function (handler, cparams) {
    const params = cparams ? JSON.parse(UTF8ToString(cparams)) : {};

    window.ysdk
      .getFlags(params)
      .then(function (flags) {
        {{{ makeDynCall('vii', 'handler') }}}(1, YGTigor.allocateJSON(flags))
      })
      .catch(function () {
        {{{ makeDynCall('vii', 'handler') }}}(0, 0)
      });
  },

  //#endregion

  //#region Server Time

  JS_GetServerTime: function (handler) {
    return window.ysdk.serverTime() ?? 0;
  }

  //#endregion
};

autoAddDeps(LisGamesSDKLib, '$YGTigor');
mergeInto(LibraryManager.library, LisGamesSDKLib);