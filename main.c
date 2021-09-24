#include <stdio.h>
#include <assert.h>
#include <gst/gst.h>

#define USE_KLV 1

static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;

    switch (GST_MESSAGE_TYPE (msg))
    {

        case GST_MESSAGE_EOS:
            g_print ("End of stream\n");
            g_main_loop_quit (loop);
            break;

        case GST_MESSAGE_ERROR: 
            {
                gchar  *debug;
                GError *error;

                gst_message_parse_error (msg, &error, &debug);
                g_free (debug);

                g_printerr ("Error: %s\n", error->message);
                g_error_free (error);

                g_main_loop_quit (loop);
                break;
            }
        default:
            break;
    }

    return TRUE;
}

int main (int argc, char *argv[])
{

#ifdef USE_KLV
    printf("WITH KLV\n");
#else
    printf("WITHOUT KLV\n");
#endif

    gst_init (&argc, &argv);
    
    GMainLoop* loop = g_main_loop_new (NULL, FALSE);

    GstElement* pipeline = gst_pipeline_new ("pipeline");
    
    GstElement* source = gst_element_factory_make ("videotestsrc", "videotestsrc");
    GstElement* encoder = gst_element_factory_make ("x265enc", "x265enc");
    GstElement* muxer = gst_element_factory_make ("mpegtsmux", "mpegtsmux");
    GstElement* filesink = gst_element_factory_make ("filesink", "filesink");

#ifdef USE_KLV
    GstElement* queue = gst_element_factory_make ("queue", "queue");
    GstElement* udpsrc = gst_element_factory_make ("udpsrc", "udpsrc");
#endif

    if(!pipeline){assert(0);}
    
    if(!source){assert(0);}
    if(!encoder){assert(0);}
    if(!muxer){assert(0);}
    if(!filesink){assert(0);}
#ifdef USE_KLV
    if(!queue){assert(0);}
    if(!udpsrc){assert(0);}
    g_object_set (G_OBJECT (udpsrc), "port", 9001, NULL);
    
#endif

    g_object_set (G_OBJECT (filesink), "location", "test.ts", NULL);
    g_object_set (G_OBJECT (source), "is-live", TRUE, NULL);

    GstBus* bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    guint bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    gst_bin_add_many (GST_BIN (pipeline), source, encoder, muxer, filesink, NULL);

#ifdef USE_KLV
    gst_bin_add_many (GST_BIN (pipeline), queue, NULL);
    gst_bin_add_many (GST_BIN (pipeline), udpsrc, NULL);
#endif



    gst_element_link (source, encoder);
    gst_element_link (encoder, muxer);
    gst_element_link (muxer, filesink);

#ifdef USE_KLV
    
    GstCaps *my_caps = gst_caps_new_simple ("meta/x-klv", 
        "sparse", G_TYPE_BOOLEAN, TRUE, 
        "parsed", G_TYPE_BOOLEAN, TRUE, 
        NULL); 
    gst_element_link (udpsrc, queue);
    gst_element_link_filtered (queue, muxer, my_caps);
#endif

    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    g_print ("Running...\n");
    g_main_loop_run (loop);

    g_print ("Returned, stopping playback\n");
    gst_element_set_state (pipeline, GST_STATE_NULL);

    g_print ("Deleting pipeline\n");
    gst_object_unref (GST_OBJECT (pipeline));
    g_source_remove (bus_watch_id);
    g_main_loop_unref (loop);

    printf("FIN\n");
}
