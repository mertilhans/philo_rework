Proje: Philosophers
Bu proje, bilgisayar bilimlerinin en ünlü eşzamanlılık (concurrency) problemlerinden biri olan Yemek Yiyen Filozoflar Problemi'nin (Dining Philosophers Problem) bir simülasyonudur. C dilinde, pthreads kütüphanesi kullanılarak geliştirilmiştir.

Proje Nedir ve Ne İşe Yarar?
Yemek Yiyen Filozoflar problemi, birden fazla işlemin (thread'in) sınırlı sayıdaki kaynağa (resource) kilitlenme (deadlock) veya aç kalma (starvation) olmadan nasıl erişmesi gerektiğini modelleyen bir alegoridir.

Alegori:

Yuvarlak bir masada oturan bir grup filozof vardır.

Her filozofun arasında birer tane çatal bulunur.

Bir filozofun yemek yiyebilmesi için hem sağındaki hem de solundaki çatalı alması gerekir.

Filozoflar hayatlarını düşünmek, yemek yemek ve uyumak döngüsünde geçirirler.

Bilgisayar Bilimindeki Karşılığı:

Filozoflar: Ayrı ayrı çalışan thread'leri temsil eder.

Çatallar: Korunması gereken paylaşılan kaynakları (mutex'leri) temsil eder.

Yemek Yemek: Bir thread'in paylaşılan bir kaynağa erişip işlem yaptığı kritik bölgeyi (critical section) temsil eder.

Bu projenin temel amacı, bu simülasyonu hayata geçirerek deadlock, starvation ve race condition gibi problemleri pthreads ve mutex'ler kullanarak çözmektir.

Programın Mimarisi (Dosya Yapısı)
Proje, her biri belirli bir sorumluluğa sahip modüler dosyalara bölünmüştür:

philo.h: Projenin kalbi. Tüm veri yapılarını (t_data, t_philo), renk kodlarını ve fonksiyon prototiplerini içerir.

main.c: Programın başlangıç noktası. Simülasyonun başlatılmasını ve sonlandırılmasını yönetir.

philo_init.c: Simülasyonun kurulumu. Komut satırı argümanlarını okur, doğrular ve gerekli tüm kaynakları (filozoflar, mutex'ler) başlatır.

philo_routine.c: Bir filozofun yaşam döngüsünü (yemek ye -> uyu -> düşün) içerir. Deadlock önleme mantığı buradadır.

philo_monitor.c: Gözlemci (monitör) thread'inin mantığı. Filozofların açlıktan ölüp ölmediğini veya herkesin doyup doymadığını denetler.

philo_utils.c & philo_utils_extra.c: Zaman yönetimi (get_time, ft_usleep) ve simülasyon durumunu yöneten yardımcı fonksiyonları içerir.

philo_clean.c: Simülasyon bittiğinde kullanılan tüm kaynakları (mutex'ler, ayrılan bellek) temizler.

Kodun Derinlikleri: Detaylı Fonksiyon Analizi
1. Kurulum Aşaması (philo_init.c)
Bu dosya, simülasyonun doğru ve güvenli bir şekilde başlamasını garanti eder.

validate_init: Ana kurulum fonksiyonudur. Argüman sayısını kontrol eder, diğer yardımcı fonksiyonları çağırır ve her şeyin yolunda olduğundan emin olur.

parse_arguments: argv'den gelen argümanları (filozof_sayısı, ölme_süresi vb.) okur, ft_atol ile sayıya çevirir ve negatif veya geçersiz değerler girilmediğinden emin olur.

init_resources: Simülasyon için gerekli tüm kaynakları hazırlar. malloc ile filozoflar ve çatallar için bellek ayırır. pthread_mutex_init ile her bir çatal ve diğer genel mutex'ler (yazdırma, ölüm kontrolü) için bir kilit oluşturur.

init_philo_and_fork: Her bir filozofu tek tek başlatır. En kritik görevlerinden biri, filozoflara çatallarını atamaktır.

2. Filozofun Yaşam Döngüsü (philo_routine.c)
Her filozof thread'i bu dosyadaki mantığı çalıştırır.

philosopher_routine: Her filozofun ana fonksiyonudur. Simülasyon bitene kadar perform_eating -> print_status("is sleeping") -> print_status("is thinking") döngüsünü çalıştırır.

perform_eating: Bir filozofun yemek yeme eylemini baştan sona yönetir.

Deadlock Çözümü: Bu fonksiyonun içindeki take_forks yardımcısı, projenin en önemli mantığını içerir. Çift ID'li filozoflar önce sağ, sonra sol çatalı alırken; tek ID'li filozoflar önce sol, sonra sağ çatalı alır. Bu asimetrik yaklaşım, tüm filozofların aynı anda sol çatallarını alıp sağdakini beklemesi gibi klasik bir deadlock senaryosunu imkansız hale getirir.

print_status: Ekrana yazdırılan her mesaj bu fonksiyondan geçer. İçindeki write_mutex, aynı anda birden fazla filozofun printf çağırmasını engelleyerek çıktıların birbirine karışmasını önler.

3. Gözlemci (Monitör) Thread'i (philo_monitor.c)
Bu thread, simülasyonun kurallara göre ilerlediğinden ve bittiğinden emin olan hakemdir.

monitor_death: Monitör thread'inin ana fonksiyonudur. Ana thread'den ayrı olarak çalışır ve sürekli olarak iki durumu kontrol eder:

Açlıktan Ölme: Herhangi bir filozofun get_time() - last_meal_time süresi time_to_die'ı aştı mı?

Doyma: Eğer [number_of_times_each_philosopher_must_eat] argümanı verildiyse, tüm filozoflar bu hedefe ulaştı mı?

check_a_philo / check_all_philos_full: Bu yardımcı fonksiyonlar, monitörün kodunu daha temiz ve Norma uygun hale getirir. Monitör, bu kontrolleri yaparken death_check_mutex'i kullanarak filozofların verilerini güvenli bir şekilde okur.

4. Yardımcı Fonksiyonlar (philo_utils*.c ve philo_clean.c)
get_time: Milisaniye cinsinden hassas zaman damgaları üreterek simülasyonun temelini oluşturur.

ft_usleep: usleep fonksiyonunun daha akıllı bir versiyonudur. Belirtilen süre boyunca beklerken, küçük aralıklarla uyanıp get_simulation_end_flag ile simülasyonun bitip bitmediğini kontrol eder. Bu, bir filozof öldüğünde diğerlerinin anında durmasını sağlar.

get_simulation_end_flag / set_simulation_end_flag: Simülasyonun bitiş bayrağını okumak ve ayarlamak için thread-safe (güvenli) bir yöntem sunar.

clean_sim: Simülasyon bittiğinde oluşturulan tüm mutex'leri pthread_mutex_destroy ile yok eder ve malloc ile ayrılan belleği free ile serbest bırakır. Bu, bellek sızıntılarını (memory leaks) önler.

Nasıl Derlenir ve Çalıştırılır?
Derleme: Proje klasöründeyken terminale make yazmak yeterlidir.

Çalıştırma:

Bash

./philo [filozof_sayısı] [ölme_süresi] [yemek_süresi] [uyuma_süresi] [yemek_hedefi(opsiyonel)]
Örnek:

Bash

./philo 4 410 200 200
Bu komut, 4 filozoflu bir simülasyon başlatır. Filozoflar 410ms aç kalırsa ölür, yemek yemeleri 200ms ve uyumaları 200ms sürer.
