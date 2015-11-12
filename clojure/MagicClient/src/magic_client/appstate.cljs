(ns magic-client.appstate
  (:require [magic-client.globals :refer [settings]]))

(defonce state (atom {:time 0
                      :settings settings}))