# captive portal for esp8266

this project implements a captive portal on an esp8266 wifi module. a captive portal is a web page that users are automatically redirected to when they connect to a public wifi network. this portal allows collecting simple login credentials or showing terms of service before granting internet access.

## features
- runs on esp8266 with littlefs filesystem
- creates a wifi access point named **Free_Wifi**
- redirects all http requests to a local web server hosting the captive portal page
- collects user credentials (email and password) via a simple form
- saves submitted data persistently on littlefs for review
- displays saved credentials on a dedicated page
- option to clear stored credentials
- responsive, modern and clean ui design with basic input validation
- supports turkish characters properly with utf-8 encoding

## use cases
- public wifi hotspots requiring login or terms acceptance
- simple authentication gateway for private or temporary networks
- educational or experimental captive portal implementation

## how it works
1. the esp8266 starts as an access point with the given ssid.
2. dns server responds to all domain queries to redirect clients to the esp’s ip.
3. the web server serves a login page with email/password inputs.
4. user submits credentials, which are saved locally.
5. device optionally blinks led to indicate data receipt.
6. admin can view or clear stored credentials via special urls.

---

# esp8266 için captive portal (turkish)

bu proje, esp8266 wifi modülü üzerinde çalışan bir captive portal uygulamasıdır. captive portal, bir kullanıcı bir wifi ağına bağlandığında otomatik olarak yönlendirilen, genellikle giriş bilgisi istenen veya kullanım şartlarının gösterildiği web sayfasıdır. internet erişimi bu sayfa üzerinden sağlanır.

## özellikler
- esp8266 ve littlefs dosya sistemi ile çalışır
- wifi erişim noktası olarak **istanbul spor** isimli ağ oluşturur
- tüm http isteklerini captive portal sayfasına yönlendirir
- kullanıcılardan email ve şifre bilgisi formu aracılığıyla toplanır
- girilen bilgiler littlefs üzerine kalıcı olarak kaydedilir
- kayıtlı girişler ayrı bir sayfada listelenir
- kayıtlı verileri temizleme seçeneği bulunur
- modern ve temiz tasarıma sahip, türkçe karakter desteği ve utf-8 kodlaması kullanılır

## kullanım alanları
- giriş yapılması gereken genel wifi noktaları
- basit kimlik doğrulama isteyen özel veya geçici ağlar
- captive portal konseptinin eğitim ve deneysel uygulamaları

## çalışma prensibi
1. esp8266 belirlenen ssid ile erişim noktası olarak çalışmaya başlar.
2. dns sunucusu gelen tüm alan adı isteklerini esp’nin ip adresine yönlendirir.
3. web sunucusu giriş formu içeren sayfayı sunar.
4. kullanıcı bilgileri formdan gönderilir ve cihazda kaydedilir.
5. veri alındığında cihaz üzerindeki led yanıp sönebilir.
6. yönetici özel url’ler üzerinden kaydedilen bilgileri görüntüleyip temizleyebilir.
