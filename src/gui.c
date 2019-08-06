#include <gtk/gtk.h>
#include "select_day.h"
#include "postgres.h"


/**Finds the child of the widget parent that has the name defined as the 
*  second parameter recursively. This function can be used to search the whole
*  programm.
*/
GtkWidget *find_child(GtkWidget* parent, const char* name)
{
	if (GTK_IS_CONTAINER(parent)){
		GList *children = gtk_container_get_children(GTK_CONTAINER(parent));

		while (children != NULL){
			if (find_child(children->data, name)){
				return find_child(children->data, name);
			}
			children = g_list_next(children);
		}

	} else if (GTK_IS_BIN(parent)){
		find_child(gtk_bin_get_child(GTK_BIN(parent)), name);
	}

	if (!strcmp(gtk_widget_get_name(parent), name)){
		return parent;
	}

	return NULL;
}


/**This function gets the needed object to add a todo entry and then calls
*  the other functions which handel the user input. This function is not 
*  being called anywhere in this code but there is a callback signal for 
*  the "+" button defined in glade/tiny-todo.glade. It also gets called if
*  one presses enter in the GtkEntry.
*/
void add_todo(GtkWidget *addEntry, GtkWindow *window)
{
	GtkEntry *textEntry = GTK_ENTRY(find_child(GTK_WIDGET(window), "text_entry"));
	const char *todo = gtk_entry_get_text(textEntry);

	if (strlen(todo) < 1){
		return;
	}
	
	GtkNotebook *mainNb = GTK_NOTEBOOK(find_child(GTK_WIDGET(window), "mainNb"));

	char pageBoxName[20];
	sprintf(pageBoxName, "pageBox_on_page_%d", gtk_notebook_get_current_page(mainNb));
	GtkBox *pageBox = GTK_BOX(find_child(GTK_WIDGET(mainNb), pageBoxName));

	GtkWidget *todoLabel = gtk_label_new(todo);
	gtk_box_pack_start(pageBox, todoLabel, FALSE, FALSE, 2);
	gtk_widget_show(todoLabel);

	gtk_entry_set_text(textEntry, "");
}


/**Creates a tab in mainNb with the tasks for the current day.
*  If there is no entry in the database for the current date
*  a new entry is created. This function is called on program
*  startup.
*/
void add_page_todays_date(GtkNotebook *mainNb)
{
	GtkBox *pageBox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
	char pageBoxName[20];
	strcpy(pageBoxName, "pageBox_on_page_0");
	gtk_widget_set_name(GTK_WIDGET(pageBox), pageBoxName);

	GtkWidget *label;
	char *date = date_as_str();
	char *prettyDate = pretty_date_as_str(date);
	char ***data = get_data(date);

	int rows = get_rows(NULL, date);

	for (int i = 0; i < rows; ++i){
		label = gtk_label_new(data[i][0]);
		gtk_box_pack_start(pageBox, label, FALSE, FALSE, 2);
	}

	gtk_notebook_append_page(mainNb, GTK_WIDGET(pageBox), gtk_label_new(prettyDate));
	
	free(date);
	free(data); 
	free(prettyDate);
}


/**Creates a GtkNotebook, adds it to a GtkBox and then calls 
*  add_page_todays_date() to fill it with some content.
*/
GtkNotebook *create_notebook(GtkBuilder *builder)
{
	GtkNotebook *mainNb = GTK_NOTEBOOK(gtk_notebook_new());
	gtk_widget_set_name(GTK_WIDGET(mainNb), "mainNb");
	gtk_notebook_set_scrollable(mainNb, TRUE);
	
	GtkBox *mainBox = GTK_BOX(gtk_builder_get_object(builder, "main_box"));
	gtk_box_pack_start(mainBox, GTK_WIDGET(mainNb), TRUE, TRUE, 2);

	add_page_todays_date(mainNb);
	
	return mainNb;
}


/**Initializes the main componenets like builder or the root widget,
*  in this case the GtkWindow main_window. Calls create_notebook().
*  This function is just here to make main() smaller and more readable.
*/
GtkWindow *create_app(GtkWindow *window)
{
	GtkBuilder *builder = gtk_builder_new();
	gtk_builder_add_from_file (builder, "src/glade/tiny-todo.glade", NULL);
	gtk_builder_connect_signals(builder, NULL);	

	window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
	gtk_window_set_title(window, "tiny-todo");

	create_notebook(builder);

	g_object_unref(builder);

	return window;
}


/**Just the entry point of the programm, nothing special happening here.
*/
int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);

	GtkWindow *window = NULL;
	window = create_app(window);

	gtk_widget_show_all(GTK_WIDGET(window));                
	gtk_main();

	return 0;
}