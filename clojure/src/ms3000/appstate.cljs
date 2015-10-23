(ns ms3000.appstate)

(defonce app-state (atom {:title "MagicShifter3000 User Interface"
                          :editor {:cols 1
                                   :rows 16
                                   :color "#ff0000"}}))
