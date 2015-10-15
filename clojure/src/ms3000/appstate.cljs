(ns ms3000.appstate)

(defonce app-state (atom {:title "MagicShifter3000 User Interface"
                          :editor {:cols 1
                                   :color "#ff0000"}

                          :leds [{:id  0 :color "#ff0000" :active false}
                                 {:id  1 :color "#ff0000" :active false}
                                 {:id  2 :color "#ff0000" :active false}
                                 {:id  3 :color "#00ff00" :active false}
                                 {:id  4 :color "#00ff00" :active false}
                                 {:id  5 :color "#00ff00" :active false}
                                 {:id  6 :color "#0000ff" :active false}
                                 {:id  7 :color "#0000ff" :active false}
                                 {:id  8 :color "#0000ff" :active false}
                                 {:id  9 :color "#ffff00" :active false}
                                 {:id 10 :color "#ffff00" :active false}
                                 {:id 11 :color "#ffff00" :active false}
                                 {:id 12 :color "#00ffff" :active false}
                                 {:id 13 :color "#00ffff" :active false}
                                 {:id 14 :color "#00ffff" :active false}
                                 {:id 15 :color "#ff00ff" :active false}]}))
