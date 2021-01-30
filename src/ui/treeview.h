#pragma once

#include <gtk/gtk.h>

class TreeView {
    public:
        GtkWidget *list;
        GtkListStore *store;
        GtkCellRenderer *renderer;
        std::vector<std::tuple<GtkTreeViewColumn*, GType>> columns;
        std::vector<GtkTreeIter*> rows;

        void update();
        void attachToVbox(GtkWidget*);

        void addColumn(std::string, GType);

        int createRow();
        void insertRow(int rowId);
        void addRowData(int rowId, const char* value, int column);

        TreeView();
};

TreeView::TreeView() {
    std::vector<std::tuple<GtkTreeViewColumn*, GType>> columns;
    std::vector<GtkTreeIter*> rows;
    this->columns = columns;
    this->rows = rows;

    this->list = gtk_tree_view_new();
    this->store = gtk_list_store_new(1, G_TYPE_STRING);
    this->renderer = gtk_cell_renderer_text_new();
}

void TreeView::addColumn(std::string title, GType type) {
    printf("%s %lu\n", title.c_str(), type);
    GtkTreeViewColumn* column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(column, title.c_str());
    gtk_tree_view_column_pack_start(column, this->renderer, false);
    gtk_tree_view_column_add_attribute(column, this->renderer, title.c_str(), this->columns.size());
    gtk_tree_view_append_column(GTK_TREE_VIEW(this->list), column);

    std::tuple<GtkTreeViewColumn*, GType> tuple(column, type);

    this->columns.push_back(tuple);

    GType gtypes[this->columns.size()];
    int i = 0;
    for (auto const& tup : this->columns) {
        GType gtype;
        std::tie(std::ignore, gtype) = tup;

        printf("gtype = %lu\n", gtype);

        gtypes[i] = gtype;

        i++;
    }

    printf("this->store = %p\n", this->store);
    printf("columns count: %lu\n", this->columns.size());

    gtk_list_store_set_column_types(
        this->store,
        this->columns.size(),
        gtypes
    );
}

void TreeView::attachToVbox(GtkWidget* vbox) {
    gtk_box_pack_start(GTK_BOX(vbox), this->list, TRUE, TRUE, 2);
};

void TreeView::update() {
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(this->list), true);
    gtk_tree_view_set_model(GTK_TREE_VIEW(this->list), GTK_TREE_MODEL(this->store));
}

int TreeView::createRow() {
    GtkTreeIter row;

    int rowId = this->rows.size();

    this->rows.push_back(&row);

    return rowId;
}

void TreeView::addRowData(int rowId, const char* value, int column) {
    GtkTreeIter* row = this->rows[rowId];
    gtk_list_store_set_value(this->store, row, column, (GValue*) value);
}

void TreeView::insertRow(int rowId) {
    GtkTreeIter* row = this->rows[rowId];
    va_list args;
    gtk_list_store_set_valist(this->store, row, args);
    gtk_list_store_append(this->store, row);
}