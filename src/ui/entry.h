#pragma once

#include <gtk/gtk.h>

typedef void(*ENTRY_ON_CHANGE)(std::shared_ptr<std::string>);

class Entry {
    public:
        GtkEntryBuffer* buffer;
        GtkWidget* entry;

        std::vector<ENTRY_ON_CHANGE> onChangeListeners;

        void attachToContainer(GtkWidget* vbox);

        void addOnChangeListener(ENTRY_ON_CHANGE);

        Entry(std::string);

    private:
        void handleChange(void*);
};

Entry::Entry(std::string initialValue) {
    this->buffer = gtk_entry_buffer_new(initialValue.c_str(), initialValue.length());
    this->entry = gtk_entry_new();

    gtk_entry_set_buffer(GTK_ENTRY(this->entry), this->buffer);
    gtk_editable_set_editable(GTK_EDITABLE(this->entry), true);

    g_signal_connect(this->entry, "changed", G_CALLBACK(+[](GtkEditable* e){
        // this->handleChange(e);
    }), gtk_label_new(""));
}

void Entry::attachToContainer(GtkWidget* container) {
    gtk_box_pack_start(GTK_BOX(container), this->entry, false, false, 0);
}

void Entry::handleChange(void* entry) {
    printf("changed\n");
}