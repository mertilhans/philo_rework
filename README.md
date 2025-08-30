# 🍝 Philosophers (philo) - Dining Philosophers Problem

Bu proje, klasik **Dining Philosophers Problem**'i pthread kütüphanesi kullanarak C dilinde çözen bir simülasyondur. Proje, thread'ler, mutex'ler ve eşzamanlılık (concurrency) kavramlarını derinlemesine öğrenmeyi amaçlar.

## 📚 Problem Açıklaması

Dining Philosophers problemi, 1965 yılında Edsger Dijkstra tarafından formüle edilmiş klasik bir bilgisayar bilimi problemidir. Problem şu şekildedir:

- **N** filozof yuvarlak bir masada oturur
- Her filozofun önünde bir tabak spagetti vardır
- Filozoflar arasında **N** adet çatal bulunur (her filozofun sağında ve solunda birer çatal)
- Bir filozof yemek yemek için **iki çatal**a ihtiyaç duyar
- Filozoflar sadece **düşünür**, **yer** ve **uyur**

## 🎯 Proje Hedefleri

- **Race condition**'ları önlemek
- **Deadlock** durumlarından kaçınmak
- **Starvation** (açlık) problemini çözmek
- Thread senkronizasyonunu öğrenmek
- Mutex kullanımını öğrenmek

## 🏗️ Proje Yapısı

```
philo/
├── Makefile
├── philo.h
├── main.c
├── philo_init.c
├── philo_monitor.c
├── philo_routine.c
├── philo_utils.c
├── philo_utils_extra.c
└── philo_clean.c
```

## 🚀 Kullanım

### Derleme
```bash
make
```

### Çalıştırma
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Parametreler
- **number_of_philosophers**: Filozof sayısı (1-200)
- **time_to_die**: Ölme süresi (milisaniye)
- **time_to_eat**: Yeme süresi (milisaniye)
- **time_to_sleep**: Uyuma süresi (milisaniye)
- **number_of_times_each_philosopher_must_eat**: (İsteğe bağlı) Her filozofun yemesi gereken yemek sayısı

### Örnek Kullanımlar
```bash
# 4 filozof, 410ms'de ölür, 200ms yer, 200ms uyur
./philo 4 410 200 200

# 5 filozof, 800ms'de ölür, 200ms yer, 200ms uyur, her biri 7 kez yemeli
./philo 5 800 200 200 7

# Tek filozof durumu (özel durum)
./philo 1 800 200 200
```

## 📋 Fonksiyon Detayları

### `main.c`
#### `begin_sim(t_data *data)`
- **Amaç**: Simülasyonu başlatır ve thread'leri oluşturur
- **İşleyiş**:
  1. Monitor thread'ini oluşturur
  2. Tüm filozof thread'lerini oluşturur
  3. Başlangıç zamanını ayarlar
  4. Thread'lerin tamamlanmasını bekler
- **Dönüş**: Başarı durumunda 0, hata durumunda 1

#### `main(int argc, char **argv)`
- **Amaç**: Program giriş noktası
- **İşleyiş**:
  1. Argümanları doğrular ve veri yapısını başlatır
  2. Simülasyonu çalıştırır
  3. Temizlik işlemlerini yapar

### `philo_init.c`
#### `ft_atol(const char *str)`
- **Amaç**: String'i long'a çevirir (güvenli versiyon)
- **Özellikler**: Hatalı girişleri kontrol eder, INT_MAX sınırını aşmaz
- **Dönüş**: Başarılı ise sayı, hatalı ise -1

#### `parse_arguments(int argc, char **argv, t_data *data)`
- **Amaç**: Komut satırı argümanlarını parse eder
- **Kontroller**: Tüm parametrelerin pozitif olduğunu doğrular

#### `init_philo_and_fork(t_data *data, int i)`
- **Amaç**: Tek bir filozof ve çatalını başlatır
- **Önemli**: Deadlock'u önlemek için çift ID'li filozoflar ters sırada çatal alır

#### `init_resources(t_data *data)`
- **Amaç**: Tüm kaynakları (mutex'ler, philosophers array) başlatır
- **Bellek yönetimi**: malloc ile bellek ayırır

#### `validate_init(int argc, char **argv, t_data *data)`
- **Amaç**: Tüm başlatma işlemlerini koordine eder
- **Düşünme süresi hesabı**: `time_to_think` otomatik hesaplanır

### `philo_monitor.c`
#### `check_a_philo(t_philo *philo)`
- **Amaç**: Tek bir filozofun ölüp ölmediğini kontrol eder
- **Kritik bölge**: `death_check_mutex` ile korunur
- **Dönüş**: Filozof öldüyse true, yaşıyorsa false

#### `check_all_philos_full(t_data *data)`
- **Amaç**: Tüm filozofların yeterli yemek yeyip yemediğini kontrol eder
- **Koşul**: `number_of_times_each_philosopher_must_eat` parametresi verilmişse çalışır

#### `monitor_death(void *arg)`
- **Amaç**: Ana monitor thread fonksiyonu
- **İşleyiş**:
  1. Simülasyon başlamasını bekler
  2. Sürekli döngüde tüm filozofları kontrol eder
  3. Ölüm veya yeterli yemek durumunda simülasyonu sonlandırır

### `philo_routine.c`
#### `print_status(t_philo *philo, char *color, char *status)`
- **Amaç**: Filozofun durumunu renkli olarak yazdırır
- **Thread-safe**: `write_mutex` ile korunur
- **Format**: `[zaman] [filozof_id] [durum]`

#### `take_forks(t_philo *philo)`
- **Amaç**: Filozofun çatalları almasını sağlar
- **Deadlock önleme**: Çift/tek ID'ye göre farklı sıralama

#### `perform_eating(t_philo *philo)`
- **Amaç**: Yemek yeme işlemini gerçekleştirir
- **İşlemler**:
  1. Çatalları al
  2. Son yemek zamanını güncelle
  3. Yemek yeme süresini bekle
  4. Çatalları bırak

#### `philosopher_routine(void *arg)`
- **Amaç**: Ana filozof thread fonksiyonu
- **Özel durumlar**: Tek filozof durumu ayrı ele alınır
- **Ana döngü**: Ye → Uyu → Düşün

### `philo_utils.c`
#### `get_time(void)`
- **Amaç**: Milisaniye cinsinden zaman alır
- **Özellik**: Sabit bir referans noktasından (my_birthday) itibaren hesaplar
- **Dönüş**: uint64_t milisaniye

#### `ft_usleep(uint64_t milliseconds)`
- **Amaç**: Milisaniye cinsinden uyuma (hassas bekleme)
- **Özellikler**: 
  - Simülasyon bitme durumunu kontrol eder
  - 250 mikrosaniyelik aralıklarla kontrol yapar

### `philo_utils_extra.c`
#### `get_data_address(t_data *data_ptr)`
- **Amaç**: Singleton pattern ile global data erişimi
- **Kullanım**: Diğer fonksiyonlardan ana veri yapısına erişim sağlar

#### `init_data_address(t_data *data)`
- **Amaç**: Singleton'u başlatır

#### `get_simulation_end_flag(void)`
- **Amaç**: Simülasyon bitiş flag'ini thread-safe olarak okur
- **Thread-safe**: `death_check_mutex` ile korunur

#### `set_simulation_end_flag(void)`
- **Amaç**: Simülasyon bitiş flag'ini thread-safe olarak ayarlar

### `philo_clean.c`
#### `print_error(char *message)`
- **Amaç**: Hata mesajını stderr'a yazdırır
- **Dönüş**: Her zaman 1 (hata kodu)

#### `clean_sim(t_data *data)`
- **Amaç**: Tüm kaynakları temizler
- **İşlemler**:
  1. Mutex'leri yok eder
  2. Malloc ile ayrılan belleği serbest bırakır

## 🔧 Teknik Detaylar

### Thread Senkronizasyonu
- **start_mutex**: Tüm thread'lerin aynı anda başlamasını sağlar
- **write_mutex**: Printf çıktılarını korur
- **death_check_mutex**: Ölüm kontrolü ve flag işlemlerini korur
- **fork mutex'leri**: Her çatal için ayrı mutex

### Deadlock Önleme Stratejisi
- Çift ID'li filozoflar: Önce sağ çatal, sonra sol çatal
- Tek ID'li filozoflar: Önce sol çatal, sonra sağ çatal
- Bu strateji dairesel beklemeyi kırar

### Renk Kodları
- 🟡 **YELLOW**: Çatal alma
- 🟢 **GREEN**: Yemek yeme
- 🔵 **BLUE**: Uyuma
- 🔷 **CYAN**: Düşünme
- 🔴 **RED**: Ölüm

### Performans Optimizasyonları
- Çift ID'li filozoflar başlangıçta `time_to_eat/2` kadar bekler (load balancing)
- `ft_usleep` fonksiyonu 250μs aralıklarla kontrol yapar
- Monitor thread 1ms aralıklarla kontrol yapar

## 🐛 Hata Durumları

### Argüman Hataları
- Yanlış argüman sayısı
- Negatif veya sıfır değerler
- INT_MAX'ı aşan değerler

### Runtime Hataları
- Thread oluşturma hatası
- Mutex başlatma hatası
- Bellek ayırma hatası

### Özel Durumlar
- **Tek filozof**: Sadece bir çatal aldıktan sonra ölür
- **Sıfır yemek sayısı**: Hiçbir filozof yemek yememeli

## 🧪 Test Senaryoları

### Temel Testler
```bash
# Hiç ölmemeli
./philo 4 410 200 200

# Bir filozof ölmeli
./philo 4 310 200 200

# Hepsi belirli sayıda yemek yemeli
./philo 5 800 200 200 7
```

### Edge Case'ler
```bash
# Tek filozof
./philo 1 800 200 200

# Çok sayıda filozof
./philo 200 800 200 200

# Çok kısa süreler
./philo 4 60 60 60
```

## 📊 Çıktı Formatı

```
[zaman_ms] [filozof_id] [durum]
```

Örnek çıktı:
```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
0 3 has taken a fork
0 3 has taken a fork
0 3 is eating
200 1 is sleeping
200 3 is sleeping
```

## 🔍 Debugging İpuçları

1. **ThreadSanitizer kullanın**: `-fsanitize=thread` flag'i ekleyin
2. **Valgrind ile test edin**: Bellek sızıntılarını kontrol edin
3. **Log çıktılarını analiz edin**: Zaman damgalarına dikkat edin
4. **Race condition tespiti**: Beklenmeyen çıktı sıralaması

## ⚠️ Dikkat Edilmesi Gerekenler

- Printf thread-safe değildir, mutex kullanın
- Mutex'leri her zaman aynı sırada alın (deadlock önleme)
- Bellek sızıntılarından kaçının
- Thread'leri mutlaka join edin
- Mutex'leri mutlaka destroy edin

## 🎓 Öğrenilen Kavramlar

- **Multi-threading**: Parallel işlem yürütme
- **Mutex**: Mutual exclusion (karşılıklı dışlama)
- **Race Condition**: Yarış durumu ve çözümleri
- **Deadlock**: Kilitlenme ve önleme teknikleri
- **Thread Synchronization**: Thread senkronizasyonu
- **Critical Section**: Kritik bölge yönetimi
