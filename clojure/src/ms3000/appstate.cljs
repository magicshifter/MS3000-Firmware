(ns ms3000.appstate)

(defonce app-state (atom {:title "MagicShifter3000 User Interface"
                          :editor {:rows 1}

                          :leds [{:id 0  :color "#ff0000" :active ""}
                                 {:id 1  :color "#ff0000" :active ""}
                                 {:id 2  :color "#ff0000" :active ""}
                                 {:id 3  :color "#00ff00" :active ""}
                                 {:id 4  :color "#00ff00" :active ""}
                                 {:id 5  :color "#00ff00" :active ""}
                                 {:id 6  :color "#0000ff" :active ""}
                                 {:id 7  :color "#0000ff" :active ""}
                                 {:id 8  :color "#0000ff" :active ""}
                                 {:id 9  :color "#ffff00" :active ""}
                                 {:id 10 :color "#ffff00" :active ""}
                                 {:id 11 :color "#ffff00" :active ""}
                                 {:id 12 :color "#00ffff" :active ""}
                                 {:id 13 :color "#00ffff" :active ""}
                                 {:id 14 :color "#00ffff" :active ""}
                                 {:id 15 :color "#ff00ff" :active ""}]}))
