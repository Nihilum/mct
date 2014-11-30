Index: boost/asio/impl/io_service.ipp
===================================================================
--- boost/asio/impl/io_service.ipp	(revision 86799)
+++ boost/asio/impl/io_service.ipp	(working copy)
@@ -36,7 +36,7 @@
 io_service::io_service()
   : service_registry_(new boost::asio::detail::service_registry(
         *this, static_cast<impl_type*>(0),
-        (std::numeric_limits<std::size_t>::max)())),
+        /*(std::numeric_limits<std::size_t>::max)()*/0)),
     impl_(service_registry_->first_service<impl_type>())
 {
 }
