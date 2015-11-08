(ns magic-ring.appstate
  (:require [magic-ring.globals :refer [settings]]))

(defonce state (atom {:time 0
                      :settings settings}))