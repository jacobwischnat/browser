#pragma once

#include <gtk/gtk.h>

class ComboText {
    public:
        GtkWidget* combo;

        void attachToVbox(GtkWidget* vbox);

        ComboText();
};

ComboText::ComboText() {
    this->combo = gtk_combo_box_text_new();
}

void ComboText::attachToVbox(GtkWidget* vbox) {
    gtk_box_pack_start(GTK_BOX(vbox), this->combo, TRUE, TRUE, 2);
}