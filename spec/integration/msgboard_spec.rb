require 'rspec'
require 'capybara/rspec'
require 'capybara/webkit'

Capybara.configure do |config|
  config.run_server = false
  config.default_driver = :webkit
  config.app_host = ENV['APP_HOST']
end

def sign_in_with(username, password)
  visit '/login'
  fill_in 'username', with: username
  fill_in 'password', with: password
  
  click_button 'Submit'
end

feature 'Sign up' do
  scenario 'with valid username and password' do 
    sign_up_with 'valid_user', 'password'

    expect(page).to have_content('New message')
    expect(page).to have_content('Logout')
  end

  scenario 'with blank password' do
    sign_up_with 'invalid_user', ''

    expect(page).to have_content('Register')
  end

  scenario 'with same username' do
    sign_up_with 'same_user', 'foobar'
    
    expect(page).to have_content('Logout')

    click_on 'Logout'

    expect(page).to have_content('Login')
    
    sign_up_with 'same_user', 'foo'

    expect(page).to have_content('Register')
  end

  def sign_up_with(username, password)
    visit '/users/new'
    fill_in 'username', with: username
    fill_in 'password', with: password
    fill_in 'password_confirmation', with: password
    click_button 'Submit'
  end
end

feature 'Sign in' do
  scenario 'with valid username and password' do 
    sign_in_with 'valid_user', 'password'

    expect(page).to have_content('New message')
    expect(page).to have_content('Logout')
  end

  scenario 'with wrong password' do
    sign_in_with 'valid_user', 'wrong_password'

    expect(page).to have_content('Login')
  end

  scenario 'with inexistent username' do
    sign_in_with 'whatever_user', 'foobar'
    
    expect(page).to have_content('Login')
  end
end

feature 'Logout' do
  scenario 'with user already logged in' do 
    sign_in_with 'valid_user', 'password'
    expect(page).to have_content('Logout')   
    
    click_on 'Logout'
    expect(page).to have_content('Login')
  end
end

feature 'New message' do
  scenario 'with user not logged in' do
    visit '/'
    expect(page).to have_content('Login')
    expect(page).not_to have_content('New message')

    visit '/messages/new'
    expect(page).to have_content('Login')
  end

  scenario 'with user already logged in' do 
    sign_in_with 'valid_user', 'password'
    expect(page).to have_content('New message')    
    
    click_on 'New message'
    expect(page).to have_content('New message')

    text = 'Neque porro quisquam est qui dolorem ipsum quia dolor sit amet'

    fill_in 'content', with: text
    click_button 'Submit'

    expect(page).to have_content(text)
  end
end