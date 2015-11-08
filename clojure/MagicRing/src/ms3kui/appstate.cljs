(ns ms3kui.appstate
  (:require [ms3kui.globals :refer [settings]]))

(defonce state (atom {:time 0
                      :settings settings}))