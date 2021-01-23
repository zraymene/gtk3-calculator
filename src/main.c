#include <stdbool.h>
#include <stdlib.h>
#include <gtk/gtk.h>


// https://coolors.co/011627-fdfffc-2ec4b6-e71d36-ff9f1c

GtkWidget* entry; 

double last = 0;
double result = 0;
char lastOperation = '\0';
bool decimalPointExists = false;
bool firstTime = true;
bool done = false;

G_MODULE_EXPORT void
on_number_button_clicked (GtkButton *button,
             gpointer   data)
{
  const gchar *button_label = gtk_button_get_label  ( button);
  const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));

  if(button_label[0] == '.')
  {
    if(decimalPointExists || entry_text[0] == '\0')
      return;
    else 
      decimalPointExists = true;
  }

  // Stop user from putting 0s if entry text already 0
  if(button_label[0] == '0' && entry_text[0] == '0' && strlen(entry_text) == 1)
    return;

  gchar *res = g_strdup_printf("%s%s", entry_text, button_label);

  gtk_entry_set_text(GTK_ENTRY(entry), res);
  g_free(res);
}

G_MODULE_EXPORT void on_sign_button_clicked()
{
  const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
  int length = strlen(entry_text);

  gchar *res;

  if(entry_text[0] == '-') // Negative
  {
    res = malloc(sizeof(gchar) * length);

    strncpy(res, entry_text + 1 , length);
    
  }else // Positive
  {
    if(entry_text[0] == '\0' || (entry_text[0] == '0' && length == 1))
      return;

    res = g_strdup_printf("-%s", entry_text);
  }

  gtk_entry_set_text(GTK_ENTRY(entry), res);
  g_free(res);
}

G_MODULE_EXPORT void on_clear_button_click(GtkButton *button)
{
  const gchar *button_label = gtk_button_get_label(button);

  switch(button_label[0])
  {
    case 'R':;
      result = 0;
      decimalPointExists = false;
      firstTime = true;
      gtk_entry_set_text(GTK_ENTRY(entry), "\0");
      break;
    case 'C':
      decimalPointExists = false;
      gtk_entry_set_text(GTK_ENTRY(entry), "\0");
      break;

    case '<':;
      const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));

      int position = strlen(entry_text) - 1;

      if(entry_text[position] == '.')
        decimalPointExists = false;

      gchar *res = g_strdup_printf("%.*s", position , entry_text);

      gtk_entry_set_text(GTK_ENTRY(entry) , res);

      g_free(res);
      break;

  }

}

void Calculate(char symbol)
{
  

  const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));

  if(entry_text[0] == '\0')
    return;

  lastOperation = symbol;

  double number = atof(entry_text);

  decimalPointExists = false;
  gtk_entry_set_text(GTK_ENTRY(entry),"\0");

  switch(symbol)
  {
    case '+':;
      result += number + last;
      break;
    case '-':;

      if(firstTime)
        result = number;
      else 
        result -= number - last;

      break;
    case '*':;

      if(firstTime)
        result = number;
      else 
        result *= number;

      break;
    case '/':;

      if(firstTime)
        result = number;
      else 
        result /= number;

      break;
  }

  if(firstTime)
    firstTime = false;
}

G_MODULE_EXPORT void on_equal_button_clicked()
{
  if(lastOperation != '\0')
    Calculate(lastOperation);

  gchar * res_str = g_strdup_printf("%lf",result);
  gtk_entry_set_text(GTK_ENTRY(entry),res_str);
  g_free(res_str);

  result = 0;
  decimalPointExists = false;
  firstTime = true;
}

G_MODULE_EXPORT void on_operator_button_clicked(GtkButton * button)
{
  const gchar *button_label = gtk_button_get_label(button);

  Calculate(button_label[0]);

}

void loadCSS(void){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path("ui/style.css"), &error);

    if(error != NULL)
      g_print("Error while loading CSS : %s", error->message);

    g_object_unref (provider);
}


int
main (int    argc,
      char **argv)
{
  GtkWidget *window;
  GtkBuilder *builder;

  gtk_init(&argc,&argv);

  loadCSS();

  builder = gtk_builder_new_from_file("ui/page.ui");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

  gtk_window_set_title (GTK_WINDOW (window), "Window");
 // gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

  entry = GTK_WIDGET(gtk_builder_get_object(builder, "input"));

  g_object_unref(builder);

  gtk_widget_show(window);

  gtk_main();

  return 0;
}