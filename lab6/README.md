Za splnění celého zadání dostanete **2 body.**

# Úkoly #

* Program z minulého cvičení modifikujte, aby vypisoval použitou šifru `(SSL_get_current_cipher)`
    * Jméno šifry získáte pomocí `(SSL_CIPHER_get_name)`
    * Vysvětlete, co znamenají jednotlivé identifikátory, z kterých se skládá takto získané jméno šifry. Zapište svá zjištění.
* Před voláním `SSL_connect` zakažte konkrétní šifru zjištěnou výše (simulujeme případ, kdy je nalezena zranitelnost šifry a je potřeba ji zakázat), zjistěte, na jaké šifře se dohodne klient a server.
    * Kdybychom např. chtěli zakázat šifru `SEED`, napíšeme `SSL_set_cipher_list(ssl, „ALL:!SEED“);`
* Vytiskněte všechny šifry, co jsou k dispozici klientovi (`SSL_get_cipher_list` volejte postupně s prioritami od 0 výše, dokud nevrátí NULL)
* Ověřte certifikát serveru
    * Po vytvoření nového kontextu `(SSL_CTX)`, zavolejte `SSL_CTX_load_verify_locations` nebo `SSL_CTX_set_default_verify_paths` (a zkontrolujte výsledek) – tím nastavíte, kde má knihovna hledat kořenové certifikáty.
    * Po vytvoření SSL spojení `(SSL_connect)`, získejte výsledek verifikace pomocí `SSL_get_verify_result` a otestujte jej.
