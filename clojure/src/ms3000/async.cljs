(ns ms3000.async
  (:require [om.core :as om :include-macros true]
            [om.dom :as dom :include-macros true]
            [cljs.core.async :as async]
            [sablono.core :as html :refer-macros [html]])
  (:require-macros [cljs.core.async.macros :refer (go)]))

(defn fetch [x]
  (let [c (async/chan)]
    (go
      ;; pretend a blocking call
      ;; wait for 2 sec
      (<! (async/timeout 2000))
      (>! c {:data "Roast peach & Parma ham salad"
             :description "This is a lovely light starter with fantastic sweet, salty and creamy flavours"}))
    c))
